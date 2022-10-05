//
// TODO(Ray02250418): Add copyright message
//

#include <iostream>

#include "AIM/priority_encoder_set.h"

Priority_Encoder_Set::Priority_Encoder_Set() {
  Priority_Encoder priority_encoder_instance;
  for (int i=0; i < specs::kPriorityEncoderNumPerAIM; i++) {
    priority_encoder_set_.push_back(priority_encoder_instance);
  }
}

void Priority_Encoder_Set::InputData(std::vector<std::vector<int>> input_data) {
  for (int i=0; i < input_data.size(); i++) {
    priority_encoder_set_.at(i).Encode(input_data.at(i));
  }
}

void Priority_Encoder_Set::Reset() {
  for (auto i : priority_encoder_set_) {
    i.Reset();
  }
}

std::vector<specs::ValidPositionPair> Priority_Encoder_Set::GetValidPositionPairs() {
  std::vector<specs::ValidPositionPair> v;
  for (auto i : priority_encoder_set_) {
    v.push_back(i.GetValidPositionPair());
  }
  return v;
}