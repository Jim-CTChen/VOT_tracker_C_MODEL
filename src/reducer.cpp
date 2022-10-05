#include "../inc/reducer.h"

#include <assert.h>
#include <iostream>
#include <math.h>
#include "../inc/specs.h"

Reducer::Reducer(int num_of_PE_set) {
  this->num_of_PE_set_ = num_of_PE_set;
  this->ResetBuffer();
}

std::vector<int> Reducer::GetOA(int& h, int& w) {
  /*
    Output OA bundle
    OA bundle is compressed by Compress()
    return vector<int> of oa, size = oa channel
    param:
    int& h: oa address
    int& w: oa address
  */
  assert(this->output_ready);
  this->PostProcess();
  this->UpdateOutputReady();
  h = this->oa_h_output_buffer_;
  w = this->oa_w_output_buffer_;
  return this->oa_output_buffer_;
}

void Reducer::SetConvConfig(specs::ConvConfig config, std::vector<int> bias) {
  this->conv_config_ = config;
  this->bias_ = bias;
  this->ResetBuffer();
  this->InitBuffer();
  this->oa_output_buffer_ = std::vector<int> (config.oa_channel);
}

void Reducer::Reduce(specs::RawData partial_sum, int ia_h, int ia_w) {
  /*
    Input:
      RawData: partial sum calculated by PE
      ia_bundle_index: the smallest ia bundle index which is currently calulating by PEs
        (this index can help reducer to determine which oa has already received all partial sum
        and therefore reducer can compress that oa bundle and output to SRAM)
    Output:
      bool: true if there's oa bundle ready to be saved to SRAM
    Functionality:
      Reduce rawdata, calculate partial sum, check output valid by processing ia bundle index
  */

  int stride = this->conv_config_.stride;
  this->ia_bundle_h_processing_in_PE_ = ia_h/stride;
  this->ia_bundle_w_processing_in_PE_ = ia_w/stride;
  // std::cerr << "Reducing, ia (h, w) = (" << ia_h << ", " << ia_w << ")" << std::endl;
  for (int i = 0; i < partial_sum.h.size(); i++) {
    int h = partial_sum.h[i];
    int w = partial_sum.w[i];
    if ((h%stride) || (w%stride)) continue; // check for redundant partial sum when stride != 1
    else {
      h /= stride;
      w /= stride;
    }
    int k = partial_sum.k[i];
    int ps = partial_sum.value[i];
    int partial_sum_index = this->BufferAddressCounter(h, w, k);

    // h, w, k should be the same if indexing is right
    assert(this->oa_partial_sum_buffer_.h[partial_sum_index] == h);
    assert(this->oa_partial_sum_buffer_.w[partial_sum_index] == w);
    assert(this->oa_partial_sum_buffer_.k[partial_sum_index] == k);

    this->oa_partial_sum_buffer_.value[partial_sum_index] += ps;
  }

  // check if output is ready
  this->UpdateOutputReady();

}

void Reducer::AllPartialSumFinish() {
  // call this function when current feature map finish calculating
  this->all_partial_sum_finish_ = true;
  this->UpdateOutputReady();
}

void Reducer::PrintReducerInfo(std::ostream& stream) {
  stream << std::endl;
  stream << ">>>=========== Reducer Buffer ============<<<" << std::endl;
  stream << "\t" << "bundle base: (" << this->buffer_bundle_base_h_ << ", " << this->buffer_bundle_base_w_ << ") = " << this->buffer_bundle_base_index_ << std::endl;
  stream << "\t" << "ia bundle processing in PE: (" << this->ia_bundle_h_processing_in_PE_ << ", " << this->ia_bundle_w_processing_in_PE_ << ")" << std::endl;
  stream << "\t" << "output ready: " << this->output_ready << std::endl;
  stream << "\t" << "PE finish all partial sum: " << this->all_partial_sum_finish_ << std::endl;
  stream << "\t" << "finish output all bundle set: " << this->finish << std::endl;

  stream << "======== Buffer info =========" << std::endl;
  stream << "\t" << "size of h: " << this->oa_partial_sum_buffer_.h.size() << std::endl;
  stream << "\t" << "size of w: " << this->oa_partial_sum_buffer_.w.size() << std::endl;
  stream << "\t" << "size of k: " << this->oa_partial_sum_buffer_.k.size() << std::endl;
  stream << "\t" << "size of value: " << this->oa_partial_sum_buffer_.value.size() << std::endl;
  stream << "\t" << "finish offset: " << this->buffer_bundle_finish_offset_ << std::endl;
  stream << "===== End of buffer info =====" << std::endl;
  stream << ">>>======= End of Reducer Buffer ========<<<" << std::endl;

}

void Reducer::PrintBuffer(bool info = false, std::ostream& stream) {
  /*
    For debug use
    Print all information in reducer buffer by given output stream
    Input:
      info (bool): print buffer size and finish offset
      stream (std::ostream&): output stream, default std::cout
  */
  stream << std::endl;
  stream << ">>>=========== Reducer Buffer ============<<<" << std::endl;
  stream << "\t" << "bundle base: (" << this->buffer_bundle_base_h_ << ", " << this->buffer_bundle_base_w_ << ") = " << this->buffer_bundle_base_index_ << std::endl;
  stream << "\t" << "ia bundle processing in PE: (" << this->ia_bundle_h_processing_in_PE_ << ", " << this->ia_bundle_w_processing_in_PE_ << ")" << std::endl;
  stream << "\t" << "output ready: " << this->output_ready << std::endl;
  stream << "\t" << "PE finish all partial sum: " << this->all_partial_sum_finish_ << std::endl;
  stream << "\t" << "finish output all bundle set: " << this->finish << std::endl;
  if (info) {
    stream << "======== Buffer info =========" << std::endl;
    stream << "\t" << "size of h: " << this->oa_partial_sum_buffer_.h.size() << std::endl;
    stream << "\t" << "size of w: " << this->oa_partial_sum_buffer_.w.size() << std::endl;
    stream << "\t" << "size of k: " << this->oa_partial_sum_buffer_.k.size() << std::endl;
    stream << "\t" << "size of value: " << this->oa_partial_sum_buffer_.value.size() << std::endl;
    stream << "\t" << "finish offset: " << this->buffer_bundle_finish_offset_ << std::endl;
    stream << "===== End of buffer info =====" << std::endl;
  }
  stream << "======= Buffer Content =======" << std::endl;
  int h, w, k, value;
  for (int i = 0; i < this->oa_partial_sum_buffer_.h.size(); i++) {
    h = this->oa_partial_sum_buffer_.h[i];
    w = this->oa_partial_sum_buffer_.w[i];
    k = this->oa_partial_sum_buffer_.k[i];
    value = this->oa_partial_sum_buffer_.value[i];
    stream << "\t" << "(" << h << ", " << w << ", " << k << "): " << value << std::endl;
  }
  stream << "=== End of Buffer Content  ===" << std::endl;
  stream << ">>>======= End of Reducer Buffer ========<<<" << std::endl;
  stream << std::endl;
}

int Reducer::BufferAddressCounter(int h, int w, int k) {
  /*
    Input: partial sum's location (h, w, k) 
    Output: address(index) for buffer in reducer

    mapping between location & address:
    partial sum indexing (h, w, k): index = [(h*oa_w+w)-1]*oa_channel+k
    partial sum indexing in buffer = index - partial_sum_base
  */

  // smallest bundle index currently stored in oa partial sum buffer
  int bundle_base = this->buffer_bundle_base_index_;
  // from bundle index to partial sum index, multiply by channel
  int partial_sum_base = bundle_base*this->conv_config_.oa_channel;
  // oa feature map size
  int oa_width = this->conv_config_.oa_size;
  int oa_channel = this->conv_config_.oa_channel;
  // std::cout << "w: " << oa_width << std::endl;
  // std::cout << "c: " << oa_channel << std::endl;
  // std::cout << "(h, w, k) = (" << h << ", " << w << ", " << k << ")" << std::endl;

  int partial_sum_index = (h*oa_width+w)*oa_channel+k;
  // std::cout << "ps base: " << partial_sum_base << std::endl;
  // std::cout << "ps index: " << partial_sum_index << std::endl;
  partial_sum_index = partial_sum_index - partial_sum_base;

  return partial_sum_index;
}

void Reducer::PostProcess() {
  /*
    Handle post-process
    Including relu, bias, quantization
    Call GetOA() after this function to get oa
  */

  // std::cerr << "---- Post Processing... ----" << std::endl;
  // std::cerr << "(h, w): (" << this->oa_bundle_h_ << ", " << this->oa_bundle_w_ << ")" << std::endl;
  // std::cerr << "buffer size h: " << this->oa_partial_sum_buffer_.h.size() << std::endl;
  // std::cerr << "buffer size w: " << this->oa_partial_sum_buffer_.w.size() << std::endl;
  // std::cerr << "buffer size value: " << this->oa_partial_sum_buffer_.value.size() << std::endl;
  int oa_channel = this->conv_config_.oa_channel;
  int bundle_size = 0;

  // handle relu, bias, quantization
  int value;
  for (int i = 0; i < oa_channel; i++) {
    value = (this->oa_partial_sum_buffer_.value[i]+this->bias_[i])*this->conv_config_.quantize_M;
    value = value >> this->conv_config_.quantize_n;

    // pass partial sum to comrpessor
    if (this->conv_config_.relu) {
      if (value > 255) value = 255;
      if (value < 0) value = 0;
    }
    else {
      if (value > 127) value = 127;
      if (value < -128) value = -128;
    }
    this->oa_output_buffer_[i] = value;
  }
  this->oa_h_output_buffer_ = this->oa_bundle_h_;
  this->oa_w_output_buffer_ = this->oa_bundle_w_;


  /////// move buffer bundle base index
  this->MoveBundleBase();

  /////// remove data
  auto h_begin = this->oa_partial_sum_buffer_.h.begin();
  this->oa_partial_sum_buffer_.h.erase(h_begin, h_begin+oa_channel);
  auto w_begin = this->oa_partial_sum_buffer_.w.begin();
  this->oa_partial_sum_buffer_.w.erase(w_begin, w_begin+oa_channel);
  auto k_begin = this->oa_partial_sum_buffer_.k.begin();
  this->oa_partial_sum_buffer_.k.erase(k_begin, k_begin+oa_channel);
  auto value_begin = this->oa_partial_sum_buffer_.value.begin();
  this->oa_partial_sum_buffer_.value.erase(value_begin, value_begin+oa_channel);

  /////// append buffer at the end for next address
  int last_h = 0;
  int last_w = 0;
  int new_h = 0;
  int new_w = 0;
  bool append = true;

  last_h = *(this->oa_partial_sum_buffer_.h.end());
  last_w = *(this->oa_partial_sum_buffer_.w.end());
  new_h = 0;
  new_w = 0;
  append = true;
  if (last_w >= (this->conv_config_.oa_size-1)) {
    if (last_h >= (this->conv_config_.oa_size-1)) { // finish, do nothing
      new_h = 0;
      new_w = 0;
      append = false;
    }
    else {
      new_h = last_h+1;
      new_w = 0;
    }
  }
  else {
    new_h = last_h;
    new_w = last_w+1;
  }

  if (append) {
    for (int c = 0; c < oa_channel; c++) {
      this->oa_partial_sum_buffer_.h.push_back(new_h);
      this->oa_partial_sum_buffer_.w.push_back(new_w);
      this->oa_partial_sum_buffer_.k.push_back(c);
      this->oa_partial_sum_buffer_.value.push_back(0);
    }
  }

  /////// move output oa bundle's (h, w)
  int oa_size = this->conv_config_.oa_size;

  if (this->oa_bundle_w_ == oa_size - 1) {
    if (this->oa_bundle_h_ == oa_size - 1) {
      this->oa_bundle_h_ = 0;
      this->oa_bundle_w_ = 0;
      this->finish = true;
    }
    else {
      this->oa_bundle_h_ += 1;
      this->oa_bundle_w_ = 0;
    }
  }
  else {
    this->oa_bundle_w_ += 1;
  }

  // std::cerr << "buffer size h: " << this->oa_partial_sum_buffer_.h.size() << std::endl;
  // std::cerr << "buffer size w: " << this->oa_partial_sum_buffer_.w.size() << std::endl;
  // std::cerr << "buffer size value: " << this->oa_partial_sum_buffer_.value.size() << std::endl;
  // std::cerr << "---- end of conpressing ---" << std::endl;
}

void Reducer::MoveBundleBase() {
  /*
    Move partial sum buffer's bundle base
    Bundle base record the smallest bundle index stored in buffer
    We can use bundle base to calculate buffer address (see more in BufferAddressCounter())
    Also, bundle base represents the next oa bundle's location
  */
  if (this->finish) {
    this->buffer_bundle_base_index_ = 0;
    this->buffer_bundle_base_h_ = 0;
    this->buffer_bundle_base_w_ = 0;
  }
  else {
    this->buffer_bundle_base_index_ += 1;
    if (this->buffer_bundle_base_w_ == this->conv_config_.oa_size-1) {
      this->buffer_bundle_base_w_ = 0;
      if (this->buffer_bundle_base_h_ == this->conv_config_.oa_size-1) {
        this->buffer_bundle_base_h_ = 0;
      }
      else {
        this->buffer_bundle_base_h_ += 1;
      }
    }
    else {
      this->buffer_bundle_base_w_ += 1;
    }
  }

  // if finish, reset base index to 0
  int oa_size = this->conv_config_.oa_size;

  if (this->oa_bundle_h_ == oa_size && this->oa_bundle_w_ == oa_size) {
    this->buffer_bundle_base_index_ = 0;
    this->buffer_bundle_base_h_ = 0;
    this->buffer_bundle_base_w_ = 0;
  }
}

void Reducer::UpdateOutputReady() {
  /*
    check if output is ready
    i.e. the index of the smallest bundle currently being proccessed in PE is 
    bigger than the bundle base by finish offset
    if it is, there's output
  */
  // finish output entire feature map
  if (this->finish) this->output_ready = false;
  // if PE finish calculating all partial sum
  else if (this->all_partial_sum_finish_) this->output_ready = true;
  // check if oa can output
  else {
    int bundle_base = this->buffer_bundle_base_index_;
    int finish_offset = this->buffer_bundle_finish_offset_;
    int ia_h = this->ia_bundle_h_processing_in_PE_;
    int ia_w = this->ia_bundle_w_processing_in_PE_;
    int process_bundle_index = ia_h*this->conv_config_.oa_size+ia_w;
    this->output_ready = ((process_bundle_index-bundle_base) >= finish_offset);
  }
}

void Reducer::InitBuffer() {
  /*
    Reducer need a buffer, (in specs::RawData form)
    size = buffer needed for waiting til one oa finish + # of partial sum in one batch
      buffer needed for waiting til one oa finish = ((k-1)*w+k)*c
    Init for each convolution layer
    FIXME: should be fixed size, and smaller, need to communicate partial sum with SRAM
  */

  int kernel = this->conv_config_.kernel_size;
  int oa_w = this->conv_config_.oa_size;
  int channel = this->conv_config_.oa_channel;
  int bundle_finish_offset = (oa_w+1)*(kernel-1)+1;
  this->buffer_bundle_finish_offset_ = bundle_finish_offset;

  // buffer needed for finishing calculating all partial sum for one oa address
  int partial_sum_finish_offset = bundle_finish_offset*channel;
  // buffer needed for input partial sum in one batch
  int buffer_for_one_batch = (this->num_of_PE_set_*this->conv_config_.kernel_size)*channel;
  int buffer_size = partial_sum_finish_offset + buffer_for_one_batch;

  if (buffer_size > oa_w*oa_w*channel) buffer_size = oa_w*oa_w*channel;


  this->oa_partial_sum_buffer_.h.clear();
  this->oa_partial_sum_buffer_.w.clear();
  this->oa_partial_sum_buffer_.k.clear();
  this->oa_partial_sum_buffer_.value.clear();

  // init buffer with correct address
  int h=0, w=0, k=0;
  for (int i = 0; i < buffer_size; i++) {
    this->oa_partial_sum_buffer_.h.push_back(h);
    this->oa_partial_sum_buffer_.w.push_back(w);
    this->oa_partial_sum_buffer_.k.push_back(k);
    this->oa_partial_sum_buffer_.value.push_back(0);
    if (k == channel-1) {
      k = 0;
      if (w == oa_w-1) {
        w = 0;
        if (h == oa_w-1); // finish
        else {
          h += 1;
        }
      }
      else {
        w += 1;
      }
    }
    else k += 1;
  }
  
}

void Reducer::ResetBuffer() {
  this->ia_bundle_h_processing_in_PE_ = 0;
  this->ia_bundle_w_processing_in_PE_ = 0;
  this->all_partial_sum_finish_ = false;
  this->buffer_bundle_base_index_ = 0;
  this->buffer_bundle_base_h_ = 0;
  this->buffer_bundle_base_w_ = 0;
  this->buffer_bundle_finish_offset_ = 0;
  this->finish = false;
  this->output_ready = false;
  this->oa_bundle_h_ = 0;
  this->oa_bundle_w_ = 0;
}
