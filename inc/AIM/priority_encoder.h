//
// TODO(Ray02250418): Add copyright message
//

#ifndef _2022_SPRING_CMODEL_INC_AIM_PRIORITY_ENCODER_H_
#define _2022_SPRING_CMODEL_INC_AIM_PRIORITY_ENCODER_H_

#include <vector>

#include "specs.h"

class Priority_Encoder{
 public:
  Priority_Encoder();
  void Encode(std::vector<int> input);
  void Reset();
  specs::ValidPositionPair GetValidPositionPair();
  int GetPosition();

 private:
  specs::ValidPositionPair valid_position_pair_;
  int position_;
};

#endif  // _2022_SPRING_CMODEL_INC_AIM_PRIORITY_ENCODER_H_
