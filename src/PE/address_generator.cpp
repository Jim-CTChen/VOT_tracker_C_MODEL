#include "PE/address_generator.h"
#include <vector>
#include <assert.h>
#include <iostream>
#include "specs.h"

AddressGenerator::AddressGenerator() {
  this->Reset();
}

void AddressGenerator::SetOASizeAndStride(int oa_size, int stride) {
  this->oa_size_ = oa_size;
  this->stride_ = stride;
}

void AddressGenerator::ComputeAddr(int num, int h, int w, std::vector<int> r, int s, std::vector<int> k) {
  /*
    param:
      num: number of valid
    function:
      calculate at most three addresses (depends on num)
      update reduction select & check if needed to accumulate partial sum from buffer
  */
  // FIXME: y should be constant since w and s is constant
  assert (num <= 3);
  assert (r.size() >= num);
  assert (k.size() >= num);

  // reset
  this->Reset();
  for (int i = 0; i < 3; i++) {
    this->oa_x_[i] = 0;
    this->oa_y_[i] = 0;
    this->oa_k_[i] = 0;
    this->output_valid_[i] = false;
  }

  // calculate addresses
  int size = this->oa_size_;
  int stride = this->stride_;
  for (int i = 0; i < num; i++) {
    int oa_x = h-r[i];
    int oa_y = w-s;
    int oa_k = k[i];

    if (oa_x < 0 || oa_y < 0) this->output_valid_[i] = false;
    else if (((oa_x/stride) < 0) || ((oa_x/stride) > (size-1)) || ((oa_y/stride) < 0) || ((oa_y/stride) > (size-1)))
      this->output_valid_[i] = false; // out of oa size bound
    else this->output_valid_[i] = true;
    this->oa_x_[i] = oa_x;
    this->oa_y_[i] = oa_y;
    this->oa_k_[i] = oa_k;
  }

  // handle reduction select and accumulate partial sum from buffer
  switch (num)
  {
    case 0:
      this->select_reduction_ = invalid;
      return;
      break;
    case 1:
      this->select_reduction_ = A_B_C;
      break;
    case 2:
      if (this->oa_x_[0] == this->oa_x_[1] &&
          this->oa_y_[0] == this->oa_y_[1] &&
          this->oa_k_[0] == this->oa_k_[1]
      ) 
        this->select_reduction_ = AB;
      else this->select_reduction_ = A_B_C;
      break;
    case 3:
      if (
        this->oa_x_[0] == this->oa_x_[1] &&
        this->oa_y_[0] == this->oa_y_[1] &&
        this->oa_k_[0] == this->oa_k_[1] &&
        this->oa_x_[1] == this->oa_x_[2] &&
        this->oa_y_[1] == this->oa_y_[2] &&
        this->oa_k_[1] == this->oa_k_[2]
      )
        this->select_reduction_ = ABC;
      else if (
        this->oa_x_[0] == this->oa_x_[1] &&
        this->oa_y_[0] == this->oa_y_[1] &&
        this->oa_k_[0] == this->oa_k_[1]
      )
        this->select_reduction_ = AB;
      else if (
        this->oa_x_[1] == this->oa_x_[2] &&
        this->oa_y_[1] == this->oa_y_[2] &&
        this->oa_k_[1] == this->oa_k_[2]
      )
        this->select_reduction_ = BC;
      else this->select_reduction_ = A_B_C;
    default:
      break;
  }
  last_x_ = this->oa_x_[num-1];
  last_y_ = this->oa_y_[num-1];
  last_k_ = this->oa_k_[num-1];
}

void AddressGenerator::GetOAAddr(std::vector<int>& x, std::vector<int>& y, std::vector<int>& k) {
  x = this->oa_x_;
  y = this->oa_y_;
  k = this->oa_k_;
}

ReductionSelect AddressGenerator::SelectReduction(std::vector<bool>& valid) {
  valid = this->output_valid_;
  return this->select_reduction_;
}



void AddressGenerator::Reset() {
  this->oa_x_ = std::vector<int>(3);
  this->oa_y_ = std::vector<int>(3);
  this->oa_k_ = std::vector<int>(3);
  this->output_valid_ = std::vector<bool>(3);
  this->select_reduction_ = dummy;
}
