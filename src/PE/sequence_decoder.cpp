#include "PE/sequence_decoder.h"

#include <vector>
#include <iostream>

#include "specs.h"

SequenceDecoder::SequenceDecoder() {
  this->Reset();
}

void SequenceDecoder::InputValPos(std::vector<specs::ValidPositionPair> v_p) {
  this->Reset();
  this->val_pos_pairs_ = v_p;
}

void SequenceDecoder::Decode() {
  int w_addr[3];
  int ia_addr[3];
  int val_count = 0;
  int val_pos_pairs_size = this->val_pos_pairs_.size();

  if (this->finish_decoding) return;

  while(val_count != 3 && this->val_pos_ptr_ != val_pos_pairs_size) {
    // std::cout << "valid position ptr: " << this->val_pos_ptr_ << std::endl;
    // std::cout << "(val, pos) = (" << this->val_pos_pairs_[this->val_pos_ptr_].valid << ", " << this->val_pos_pairs_[this->val_pos_ptr_].pos << ")" << std::endl; 
    if (this->val_pos_pairs_[this->val_pos_ptr_].valid) {
      w_addr[val_count] = this->val_pos_ptr_+this->invalid_offset_;
      ia_addr[val_count] = this->val_pos_pairs_[this->val_pos_ptr_].pos;
      val_count++;
    }
    else {
      this->invalid_offset_ += this->val_pos_pairs_[this->val_pos_ptr_].pos-1;
    }
    this->val_pos_ptr_++;
  }
  if (this->val_pos_ptr_ == val_pos_pairs_size) this->finish_decoding = true;

  this->num_of_valid_addr = val_count;
  for (int i = 0; i < val_count; i++) {
    this->w_addr_[i] = w_addr[i];
    this->ia_addr_[i] = ia_addr[i];
  }

}

int SequenceDecoder::GetAddr(std::vector<int>& w_addr, std::vector<int>& ia_addr) {
  /*
    return number of valid address & ia/w addresses
    and call this->Decode() to generate next batch of addresses
  */
  w_addr = this->w_addr_;
  ia_addr = this->ia_addr_;
  // this->Decode();
  return this->num_of_valid_addr;
}

void SequenceDecoder::Reset() {
  this->val_pos_pairs_.clear();
  this->finish_decoding = false;
  this->val_pos_ptr_ = 0;
  this->num_of_valid_addr = 0;
  this->invalid_offset_ = 0;
  this->w_addr_ = std::vector<int>(3);
  this->ia_addr_ = std::vector<int>(3);
}