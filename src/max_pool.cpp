#include <vector>
#include <iostream>

#include "../inc/max_pool.h"

// FSM
int MaxPool::S_IDLE = 0;
int MaxPool::S_IN_CONFIG = 1;
int MaxPool::S_CHECK_CAN_OUTPUT = 2;
int MaxPool::S_IN_BUNDLE = 3;
int MaxPool::S_MAX_POOL = 4;
int MaxPool::S_OUTPUT = 5;

MaxPool::MaxPool() {
  this->state_ = this->S_IDLE;
  this->Start();
  this->output_bundle_h_ = 0;
  this->output_bundle_w_ = 0;
  this->bundle_base_index_ = 0;
  this->request = false;
  this->request_bundle_index = 0;
  this->counter_ = 0;
  this->can_maxpool_ = false;
}

void MaxPool::Start() {
  if (this->state_ == this->S_IDLE) this->state_ = this->S_IN_CONFIG;
}

void MaxPool::SetConfig(specs::MaxPoolConfig m_config) {
  if (this->state_ == this->S_IN_CONFIG) {
    this->max_pool_config_ = m_config;
    this->state_ = S_CHECK_CAN_OUTPUT;
    this->buffer_bundle_finish_offset_ = (m_config.max_pool_kernel_size-2)*(m_config.ia_size+1);
    this->bundle_base_index_ = m_config.ia_size+1; // stride = 2, start from (1, 1) in oa feature map
    this->ResetBuffer();
  }
}

void MaxPool::SetReducerIABundleIndex(int index) {
  if (this->state_ == S_CHECK_CAN_OUTPUT) {
    this->ia_bundle_index_finish_reduce_ = index;
    this->CheckCanMaxPool();
    if (this->can_maxpool_) {
      this->state_ = S_IN_BUNDLE;
      this->counter_ = 0;
      this->UpdateRequest();
    }
  }
}

void MaxPool::CheckCanMaxPool() {
  if ((this->ia_bundle_index_finish_reduce_ - this->bundle_base_index_) >= this->buffer_bundle_finish_offset_) {
    // can do max pool and output
    this->can_maxpool_ = true;
  }
}

void MaxPool::UpdateRequest() {
  /*
    update this->request & this->request_bundle_index
    0  1  2
    3  4  5
    6  7  8
    this->bundle_base_index_ = 4

    Ceil mode:
      Since currently all max pool kernel = 3,
      we only need to consider edge cases for oa_size being even number,
      which means when the kernel move to the right-most or bottom-most position, 
      we need to pad. (i.e. pytorch's ceil mode)
      (when right-most, pad 2, 5, 8; bottom-most, pad 6, 7, 8)
      Since it's max-pooling, I pad the value of position 4.
  */
  int base = this->bundle_base_index_;
  bool right_edge = !(this->max_pool_config_.ia_size % 2) && // if it's even, and reach edge, pad value of position 4
                    (this->output_bundle_w_ == this->max_pool_config_.oa_size-1); 
  bool bottom_edge = !(this->max_pool_config_.ia_size % 2) && // if it's even, and reach edge, pad value of position 4
                     (this->output_bundle_h_ == this->max_pool_config_.oa_size-1); 
  int w = this->max_pool_config_.ia_size;
  if (this->state_ == this->S_IN_BUNDLE) {
    this->request = true;
    switch (this->counter_)
    {
    case 0:
      this->request_bundle_index = base-w-1;
      break;
    case 1:
      this->request_bundle_index = base-w;
      break;
    case 2:
      if (right_edge) this->request_bundle_index = base;
      else this->request_bundle_index = base-w+1;
      break;
    case 3:
      this->request_bundle_index = base-1;
      break;
    case 4:
      this->request_bundle_index = base;
      break;
    case 5:
      if (right_edge) this->request_bundle_index = base;
      else this->request_bundle_index = base+1;
      break;
    case 6:
      if (bottom_edge) this->request_bundle_index = base;
      else this->request_bundle_index = base+w-1;
      break;
    case 7:
      if (bottom_edge) this->request_bundle_index = base;
      else this->request_bundle_index = base+w;
      break;
    case 8:
      if (right_edge || bottom_edge) this->request_bundle_index = base;
      else this->request_bundle_index = base+w+1;
      break;
    default:
      this->request = false;
      break;
    }
  }
  else {
    this->request = false;
    this->request_bundle_index = 0;
  }
}

void MaxPool::SendBundle(specs::InputActivationBundle ia_bundle) {
  if (this->state_ == this->S_IN_BUNDLE) {
    this->bundle_buffer_[this->counter_] = ia_bundle;
    if (this->counter_ == 8) {
      this->counter_ = 0;
      this->state_ = this->S_MAX_POOL;
      this->UpdateRequest();
      this->MaxPooling();
    }
    else {
      this->counter_ += 1;
      this->UpdateRequest();
    }
  }
}

void MaxPool::ResetBuffer() {
  this->bundle_buffer_.clear();
  this->bundle_buffer_.resize(9); // we only have 3x3 max-pooling
}

void MaxPool::MaxPooling() {
  specs::InputActivationBundle oa_bundle;
  int size = 0;
  std::vector<int> counter = std::vector<int>(9);
  std::fill(counter.begin(), counter.end(), 0);
  std::vector<int> temp = std::vector<int>(9);
  bool all_zero = true;
  if (this->state_ == S_MAX_POOL) {
    // FIXME sth wrong!
    for (int c = 0; c < this->max_pool_config_.ia_channel; c++) {
      for (int i = 0; i < 9; i++) {
        if (this->bundle_buffer_[i].depth == 0) temp[i] = 0;
        else if (this->bundle_buffer_[i].channel_idx[counter[i]] == c) {
          temp[i] = this->bundle_buffer_[i].data[counter[i]];
          if (counter[i] != this->bundle_buffer_[i].depth-1) counter[i] += 1;
        }
        else temp[i] = 0;
      }
      // if (this->output_bundle_w_ == 2) {
      //   for (auto it : temp) {
      //     std::cerr << it << " ";
      //   }
      //   std::cerr << std::endl;
      // }
      int data = this->Max(temp);
      if (data == 0) continue;
      else {
        size += 1;
        oa_bundle.channel_idx.push_back(c);
        oa_bundle.data.push_back(data);
      }
    }
    oa_bundle.depth = size;
    oa_bundle.h = this->output_bundle_h_;
    oa_bundle.w = this->output_bundle_w_;
    this->output_oa_bundle_ = oa_bundle;
    this->UpdateBundleBase();
    this->state_ = this->S_OUTPUT;
    this->UpdateOutputReady();
  }
}

int MaxPool::Max(std::vector<int> values) {
  int max = values[0];
  for (int i = 1; i < values.size(); i++) {
    if (values[i] > max) max = values[i];
  }
  return max;
}

void MaxPool::UpdateOutputReady() {
  if (this->state_ == S_OUTPUT) this->output_ready = true;
  else this->output_ready = false;
}

specs::InputActivationBundle MaxPool::GetOABundle() {
  if (this->state_ == this->S_OUTPUT) {
    if (this->bundle_base_index_ == 0) { // finish all output
      this->state_ = this->S_IN_CONFIG;
    }
    else this->state_ = this->S_CHECK_CAN_OUTPUT;
    this->UpdateOutputReady();
    return this->output_oa_bundle_;
  }
  else {
    specs::InputActivationBundle a;
    return a;
  }
}

void MaxPool::UpdateBundleBase() {
  int stride = this->max_pool_config_.max_pool_stride;
  int kernel = this->max_pool_config_.max_pool_kernel_size;
  int margin = kernel/2;
  int w = this->max_pool_config_.ia_size;
  if (this->state_ == S_MAX_POOL) {
    // move output h, w & bundle base
    if (this->output_bundle_w_ == this->max_pool_config_.oa_size-1) {
      if (this->output_bundle_h_ == this->max_pool_config_.oa_size-1) {
        // finish
        this->output_bundle_w_ = 0;
        this->output_bundle_h_ = 0;
        this->bundle_base_index_ = 0;
      }
      else {
        this->output_bundle_w_ = 0;
        this->output_bundle_h_ += 1;
        this->bundle_base_index_ = w*(this->output_bundle_h_*stride+margin)+margin;
      }
    }
    else {
      this->output_bundle_w_ += 1;
      this->bundle_base_index_ += stride;
    }
  }
}

void MaxPool::PrintStatus(std::ostream& stream = std::cerr) {
  stream << std::endl;
  stream << "========= max pool status ========" << std::endl;
  stream << "\t" << "state: " << this->state_ << std::endl;
  stream << "\t" << "can max pool: " << this->can_maxpool_ << std::endl;
  stream << "\t" << "output ready: " << this->output_ready << std::endl;
  stream << "\t" << "bundle base: " << this->bundle_base_index_ << std::endl;
  stream << "\t" << "index finish reduce: " << this->ia_bundle_index_finish_reduce_ << std::endl;
  stream << "\t" << "can output offset: " << this->buffer_bundle_finish_offset_ << std::endl;
  stream << "\t" << "output h: " << this->output_bundle_h_ << std::endl;
  stream << "\t" << "output w: " << this->output_bundle_w_ << std::endl;
  stream << "\t" << "counter: " << this->counter_ << std::endl;
  stream << "\t" << "requesting: " << this->request << std::endl;
  stream << "\t" << "request bundle index: " << this->request_bundle_index << std::endl;
  

  stream << "\t" << "=== print buffer ===" << std::endl;
  for (int i = 0; i < this->bundle_buffer_.size(); i++) {
    stream << "\t\t" << "buffer " << i << ": (h, w) = (" << this->bundle_buffer_[i].h << ", " << this->bundle_buffer_[i].w << ")" << std::endl;
    for (int j = 0; j < this->bundle_buffer_[i].channel_idx.size(); j++) {
      stream << "\t\t" << "<channel, data>: <" << this->bundle_buffer_[i].channel_idx[j] << ", " << this->bundle_buffer_[i].data[j] << ">" << std::endl;
    }
  }
  stream << "\t" << "====================" << std::endl;

  stream << "==================================" << std::endl;

}


