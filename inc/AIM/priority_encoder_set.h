//
// TODO(Ray02250418): Add copyright message
//

#ifndef _2022_SPRING_CMODEL_INC_AIM_PRIORITY_ENCODER_SET_H_
#define _2022_SPRING_CMODEL_INC_AIM_PRIORITY_ENCODER_SET_H_

#include <vector>

#include "specs.h"
#include "AIM/priority_encoder.h"

class Priority_Encoder_Set{
 public:
  Priority_Encoder_Set();
  void InputData(std::vector<std::vector<int>>);
  void Reset();
  std::vector<specs::ValidPositionPair> GetValidPositionPairs();
 private:
  std::vector<Priority_Encoder> priority_encoder_set_;
};

#endif  // _2022_SPRING_CMODEL_INC_AIM_PRIORITY_ENCODER_SET_H_
