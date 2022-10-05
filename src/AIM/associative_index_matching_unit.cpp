//
// TODO(Ray02250418): Add copyright message
//

#include <iostream>

#include "AIM/associative_index_matching_unit.h"

AssociativeIndexMatchingUnit::AssociativeIndexMatchingUnit()
  : comparator_array_() {
}

void AssociativeIndexMatchingUnit::InputBundle(
    const specs::InputActivationBundle input_activation_bundle,
    const specs::WeightBundle weight_bundle
  ) {
  valid_position_pairs_.clear();
  ia_bundle_ = input_activation_bundle;
  w_bundle_ = weight_bundle;

  CompareWholeBundle();
}

void AssociativeIndexMatchingUnit::CompareWholeBundle() {
  std::vector<std::vector<int>> comparator_array_output;
  std::vector<specs::ValidPositionPair> encoder_set_output;
  ia_c_idx_ = ia_bundle_.channel_idx;
  w_c_idx_ = w_bundle_.channel_idx;
  for (int i = 0; i < w_bundle_.depth; i += specs::kComparatorArraySize) {
    int last_w = std::min<int>(w_bundle_.depth, i + specs::kComparatorArraySize);
    std::vector<int> w_comparator_input(w_c_idx_.begin() + i, w_c_idx_.begin() + last_w);
    for (int j = 0; j < ia_bundle_.depth; j += specs::kComparatorArraySize) {
      int last_ia = std::min<int>(ia_bundle_.depth, j + specs::kComparatorArraySize);
      std::vector<int> ia_comparator_input(ia_c_idx_.begin() + j, ia_c_idx_.begin() + last_ia);
      comparator_array_output = comparator_array_.Compare(w_comparator_input, ia_comparator_input);
      priority_encoder_set_.InputData(comparator_array_output);
      encoder_set_output = priority_encoder_set_.GetValidPositionPairs();
    }
    valid_position_pairs_.insert(valid_position_pairs_.begin(), encoder_set_output.begin(), encoder_set_output.end());
    priority_encoder_set_.Reset();
  }
}

std::vector<specs::ValidPositionPair> AssociativeIndexMatchingUnit::GetValidPositionPairs() {
  return valid_position_pairs_;
}
