//
// TODO(Ray02250418): Add copyright message
//

#include <iostream>

#include "AIM/comparator_array.h"

std::vector<std::vector<int>> ComparatorArray::Compare(
    std::vector<int> weight_channel_idx,
    std::vector<int> input_activation_channel_idx
  ) {
  std::vector<std::vector<int>> comparator_output;
  std::vector<int> row_output;

  for (int i=0; i < weight_channel_idx.size(); i++) {
    row_output.clear();
    for (int j=0; j < input_activation_channel_idx.size(); j++) {
      row_output.push_back(weight_channel_idx.at(i) == input_activation_channel_idx.at(j) ? 1 : 0);
    }
    comparator_output.push_back(row_output);
  }
  return comparator_output;
}
