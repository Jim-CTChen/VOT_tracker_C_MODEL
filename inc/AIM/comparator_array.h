//
// TODO(Ray02250418): Add copyright message
//

#ifndef _2022_SPRING_CMODEL_INC_AIM_COMPARATOR_ARRAY_H_
#define _2022_SPRING_CMODEL_INC_AIM_COMPARATOR_ARRAY_H_

#include <vector>

#include "specs.h"

class ComparatorArray{
 public:
  ComparatorArray() {}
  std::vector<std::vector<int>> Compare(std::vector<int>, std::vector<int>);
 private:
  int comparator_input_activation_channel_idx_[specs::kComparatorArraySize];
  int comparator_weight_channel_idx_[specs::kComparatorArraySize];
};

#endif  // _2022_SPRING_CMODEL_INC_AIM_COMPARATOR_ARRAY_H_
