//
// TODO(Ray02250418): Add copyright message
//

#include "PE/pe.h"
#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>
#include "specs.h"
#include "PE/address_generator.h"
#include "PE/sequence_decoder.h"


PE::PE() {
  this->finish = false;
  this->Reset();
}

void PE::SetConvConfig(specs::ConvConfig conv_config) {
  this->conv_config_ = conv_config;
  this->addr_generator_.SetOASizeAndStride(conv_config.oa_size, conv_config.stride);
  this->Reset();
}

void PE::InputIABundle(specs::InputActivationBundle ia_bundle) {
  this->ia_bundle_buffer_ = ia_bundle;
  this->Reset();
}

void PE::InputWBundle(specs::WeightBundle w_bundle) {
  this->w_bundle_buffer_ = w_bundle;
  this->Reset();
}

void PE::InputValidPosition(std::vector<specs::ValidPositionPair> val_pos_pairs) {
  this->seq_decoder_.InputValPos(val_pos_pairs);
}

void PE::Process(std::ostream& stream) {
  /*
    process three multiplications at a time
  */
  bool debug = false;
  if (this->seq_decoder_.finish_decoding) {
    if (debug) stream << "PE Finish!!!!" << std::endl;
    this->finish = true;
    this->UpdateOutputValid();
    return;
  }

  // decoding w addr & ia addr
  if (debug) stream << "sequence decoder..." << std::endl;
  this->seq_decoder_.Decode();
  std::vector<int> w_addr;
  std::vector<int> ia_addr;
  int num_of_valid_addr = 0;
  if (debug) stream << "sequence decoder get address" << std::endl;
  num_of_valid_addr = this->seq_decoder_.GetAddr(w_addr, ia_addr);
  if (debug) 
    for (int i = 0; i < 3; i++) {
      stream << "(num, w addr, ia addr): (" << num_of_valid_addr << ", " << w_addr[i] << ", " << ia_addr[i] << ")" << std::endl;
    }
  assert(w_addr.size() == 3);
  assert(ia_addr.size() == 3);

  // fetch w and ia
  if (debug) stream << "fetching w & ia" << std::endl;
  std::vector<int> weight = std::vector<int>(3);
  std::vector<int> ia = std::vector<int>(3);
  for (int i = 0; i < num_of_valid_addr; i++) {
    weight[i] = (this->w_bundle_buffer_.data[w_addr[i]]);
    ia[i] = (this->ia_bundle_buffer_.data[ia_addr[i]]);
    // channel index should be the same
    assert (this->w_bundle_buffer_.channel_idx[w_addr[i]] == this->ia_bundle_buffer_.channel_idx[ia_addr[i]]);
  }
  if (debug) 
    for (int i = 0; i < 3; i++) {
      stream << "<ia, w> = <" << ia[i] << ", " << weight[i] << ">" << std::endl;
    }

  // fetch h, w, r, s, k
  int ia_h = this->ia_bundle_buffer_.h;
  int ia_w = this->ia_bundle_buffer_.w;
  int w_s = this->w_bundle_buffer_.s;
  std::vector<int> w_r = std::vector<int>(3);
  std::vector<int> w_k = std::vector<int>(3);
  for (int i = 0; i < num_of_valid_addr; i++) {
    while (this->prk_ptr_ < this->w_bundle_buffer_.pos_ptr.size()-1 && w_addr[i] >= this->w_bundle_buffer_.pos_ptr[this->prk_ptr_+1]) {
      // next r_k pair
      this->prk_ptr_++;
    }
    if (debug) stream << "prk ptr: " << this->prk_ptr_ << "  (r, k) = (" << this->w_bundle_buffer_.r_idx[this->prk_ptr_] << ", " << this->w_bundle_buffer_.k_idx[this->prk_ptr_] << ")" << std::endl;
    w_r[i] = (this->w_bundle_buffer_.r_idx[this->prk_ptr_]);
    w_k[i] = (this->w_bundle_buffer_.k_idx[this->prk_ptr_]);
  }
  
  if (debug)  stream << "compute address" << std::endl;
  if (debug) 
    for (int i = 0; i < num_of_valid_addr; i++) {
      stream << "(h, w, r, s, k) = (" << ia_h << ", " << ia_w << ", " << w_r[i] << ", " << w_s << ", " << w_k[i] << ")" << std::endl;
    }

  // generate oa address (x, y, k), reduction select and accumulate psum
  this->addr_generator_.ComputeAddr(num_of_valid_addr, ia_h, ia_w, w_r, w_s, w_k);
  ReductionSelect reduction_select;
  bool accumulate_psum;
  std::vector<bool> output_valid;
  std::vector<int> gen_x;
  std::vector<int> gen_y;
  std::vector<int> gen_k;
  gen_x.resize(3);
  gen_y.resize(3);
  gen_k.resize(3);
  this->addr_generator_.GetOAAddr(gen_x, gen_y, gen_k);
  reduction_select = this->addr_generator_.SelectReduction(output_valid);
  if (debug) 
    for (int i = 0; i < 3; i++) {
      stream << "receive address from address generator (x, y, k, valid) = (" << gen_x[i] << ", " << gen_y[i] << ", " << gen_k[i] << ", " << output_valid[i] << ")" << std::endl;
    }

  if (debug) stream << "multiplication" << std::endl;
  // multiplication
  // output_valid shows whether the psum can use
  // if invalid, there're two cases
  // 1. out of oa bound
  // 2. no enough valid position pair, i.e. < 3
  int A = output_valid[0] ? weight[0]*ia[0] : 0;
  int B = output_valid[1] ? weight[1]*ia[1] : 0;
  int C = output_valid[2] ? weight[2]*ia[2] : 0;
  if (debug) {
    stream << "A, B, C: " << A << ", " << B << ", " << C << std::endl;
  }

  // reduction & accumulate
  int out[3];
  int out_x[3] = {0};
  int out_y[3] = {0};
  int out_k[3] = {0};
  switch (reduction_select)
  {
    case ABC:
      out[0] = A+B+C;
      out[1] = 0;
      out[2] = 0;
      out_x[0] = gen_x[0];
      out_y[0] = gen_y[0];
      out_k[0] = gen_k[0];
      break;
    case AB:
      out[0] = A+B;
      out[1] = C;
      out[2] = 0;
      out_x[0] = gen_x[0];
      out_y[0] = gen_y[0];
      out_k[0] = gen_k[0];
      out_x[1] = gen_x[2];
      out_y[1] = gen_y[2];
      out_k[1] = gen_k[2];
      break;
    case BC:
      out[0] = A;
      out[1] = B+C;
      out[2] = 0;
      out_x[0] = gen_x[0];
      out_y[0] = gen_y[0];
      out_k[0] = gen_k[0];
      out_x[1] = gen_x[1];
      out_y[1] = gen_y[1];
      out_k[1] = gen_k[1];
      break;
    case A_B_C:
      out[0] = A;
      out[1] = B;
      out[2] = C;
      out_x[0] = gen_x[0];
      out_y[0] = gen_y[0];
      out_k[0] = gen_k[0];
      out_x[1] = gen_x[1];
      out_y[1] = gen_y[1];
      out_k[1] = gen_k[1];
      out_x[2] = gen_x[2];
      out_y[2] = gen_y[2];
      out_k[2] = gen_k[2];
      break;
    default:
      out[0] = 0;
      out[1] = 0;
      out[2] = 0;
      break;
  }
  if (debug) stream << "output" << std::endl;
  // out[0]
  accumulate_psum = (
    this->output_x_.size() > 0 &&
    this->output_x_.back() == out_x[0] &&
    this->output_y_.back() == out_y[0] &&
    this->output_k_.back() == out_k[0]
  );
  if (accumulate_psum) {
    // accumulate with psum stored in output buffer
    this->output_value_.back() += out[0];
    // address should be the same as the psum's stored in output buffer
    assert(this->output_x_.back() == out_x[0]);
    assert(this->output_y_.back() == out_y[0]);
    assert(this->output_k_.back() == out_k[0]);
  }
  else if (out[0] != 0) { 
    this->output_value_.push(out[0]);
    this->output_x_.push(out_x[0]);
    this->output_y_.push(out_y[0]);
    this->output_k_.push(out_k[0]);
  }

  // out[1,2]
  for (int i = 1; i < 3; i++) {
    if (out[i] != 0) { 
      this->output_value_.push(out[i]);
      this->output_x_.push(out_x[i]);
      this->output_y_.push(out_y[i]);
      this->output_k_.push(out_k[i]);
    }
  }

  // update output valid
  // this->PrintFIFO(stream);
  // std::cout << "output fifo size: " << this->output_value_.size() << std::endl;
  if (debug) stream << "update output valid" << std::endl;
  this->UpdateOutputValid();
  if (debug) stream << "end process()" << std::endl;
}

void PE::PrintFIFO(std::ostream& stream) {
  std::queue<int> x = this->output_x_;
  std::queue<int> y = this->output_y_;
  std::queue<int> k = this->output_k_;
  std::queue<int> value = this->output_value_;
  // stream << "Printing output FIFO in PE" << std::endl;
  for (int i = 0; i < this->output_value_.size(); i++) {
    stream << "the " << i << "'s element in fifo: (x, y, k, value) = (" << x.front() << ", " << y.front() << ", " << k.front() << ", " << value.front() << ")" << std::endl;
    x.pop();
    y.pop();
    k.pop();
    value.pop();
  }
}

specs::RawData PE::GetPsum(std::ostream& stream) {
  /*
    currently only output one partial sum
  */
  specs::RawData psum;
  // this->PrintFIFO(stream);
  if (this->output_valid) {
    psum.value.push_back(this->output_value_.front());
    psum.h.push_back(this->output_x_.front());
    psum.w.push_back(this->output_y_.front());
    psum.k.push_back(this->output_k_.front());
    this->output_value_.pop();
    this->output_x_.pop();
    this->output_y_.pop();
    this->output_k_.pop();
  }
  this->UpdateOutputValid();
  return psum;
}

void PE::UpdateOutputValid() {
  if (this->finish) this->output_valid = this->output_value_.size() > 0;
  else this->output_valid = this->output_value_.size() > 1;
}

void PE::Reset() {
  this->seq_decoder_.Reset();
  this->addr_generator_.Reset();
  this->output_valid = false;
  this->finish = false;
  this->prk_ptr_ = 0;
  this->output_value_ = std::queue<int>();
  this->output_x_ = std::queue<int>();
  this->output_y_ = std::queue<int>();
  this->output_k_ = std::queue<int>();
}
