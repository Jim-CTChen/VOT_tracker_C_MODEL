# include "compressor.h"

#include <vector>
#include <assert.h>
#include "specs.h"

Compressor::Compressor() {
  return;
}

void Compressor::SetOAChannel(int size) {
  this->oa_channel_ = size;
}

void Compressor::InputRaw(int h, int w, std::vector<int> raw) {
  assert(raw.size() == this->oa_channel_);
  this->oa_h_ = h;
  this->oa_w_ = w;
  this->raw_data_buffer_ = raw;
  return;
}

specs::InputActivationBundle Compressor::Compress() {
  // only compress for the first h, w
  specs::InputActivationBundle ia_bundle;

  ia_bundle.h = this->oa_h_;
  ia_bundle.w = this->oa_w_;
  ia_bundle.depth = 0;

  for (int i = 0; i < this->oa_channel_; i++) {
    if (this->raw_data_buffer_[i] == 0) continue;

    ia_bundle.depth++;
    ia_bundle.channel_idx.push_back(i);
    ia_bundle.data.push_back(this->raw_data_buffer_[i]);
  }

  return ia_bundle;
}