//
// TODO(Ray02250418): Add copyright message
//

#include <iostream>

#include "AIM/priority_encoder.h"

Priority_Encoder::Priority_Encoder()
  : valid_position_pair_() {
  valid_position_pair_.valid = 0;
  valid_position_pair_.pos = -1;
  position_ = 0;
}

void Priority_Encoder::Reset() {
  valid_position_pair_.valid = 0;
  valid_position_pair_.pos = -1;
  position_ = 0;
}

void Priority_Encoder::Encode(std::vector<int> input) {
  for (int i=0; i < input.size(); i++) {
    if (input.at(i) == 1) {
      // std::cout << "Find '1' at " << position_ << std::endl;
      valid_position_pair_.valid = 1;
      valid_position_pair_.pos = position_;
      return;
    }
    position_++;
  }
}

specs::ValidPositionPair Priority_Encoder::GetValidPositionPair() {
  return valid_position_pair_;
}

int Priority_Encoder::GetPosition() {
  return position_;
}
