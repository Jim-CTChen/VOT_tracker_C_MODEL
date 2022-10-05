//
// TODO(Ray02250418): Add copyright message
//

#ifndef _2022_SPRING_CMODEL_INC_AIM_ASSOCIATIVE_INDEX_MATCHING_UNIT_H_
#define _2022_SPRING_CMODEL_INC_AIM_ASSOCIATIVE_INDEX_MATCHING_UNIT_H_

#include <vector>

#include "specs.h"
#include "AIM/comparator_array.h"
#include "AIM/priority_encoder_set.h"

class AssociativeIndexMatchingUnit{
 public:
  AssociativeIndexMatchingUnit();
  void InputBundle(const specs::InputActivationBundle, const specs::WeightBundle);
  std::vector<specs::ValidPositionPair> GetValidPositionPairs();

 private:
  void CompareWholeBundle();

  specs::InputActivationBundle ia_bundle_;
  std::vector<int> ia_c_idx_ = ia_bundle_.channel_idx;
  specs::WeightBundle w_bundle_;
  std::vector<int> w_c_idx_ = w_bundle_.channel_idx;

  ComparatorArray comparator_array_;
  Priority_Encoder_Set priority_encoder_set_;
  std::vector<specs::ValidPositionPair> valid_position_pairs_;
};

#endif  // _2022_SPRING_CMODEL_INC_AIM_ASSOCIATIVE_INDEX_MATCHING_UNIT_H_
