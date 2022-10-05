//
// TODO(Ray02250418): Add copyright message
//

#include <gtest/gtest.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#include "priority_encoder.h"
#include "utils.h"

TEST(priority_encoder, Init) {
  Priority_Encoder priority_encoder;
  EXPECT_EQ(priority_encoder.GetValidPositionPair().valid, 0);
  EXPECT_EQ(priority_encoder.GetValidPositionPair().pos, -1);
  EXPECT_EQ(priority_encoder.GetPosition(), 0);
}

TEST(priority_encoder, Encode_and_Rest) {
  // Create input data and true output
  int input_length = rand_r(&utils::seed) % 100;
  std::vector<int> input_data(input_length, 0);
  bool match = rand_r(&utils::seed) % 2 == 1 ? true : false;
  int match_pos = match ? rand_r(&utils::seed) % input_length : -1;
  if (match) {
    input_data.at(match_pos) = 1;
  }

  // Get modeule output
  Priority_Encoder priority_encoder;
  for (int i = 0; i < input_length; i += specs::kComparatorArraySize) {
    int last = std::min<int>(input_length, i + specs::kComparatorArraySize);
    std::vector<int> encoder_input(input_data.begin() + i, input_data.begin() + last);
    priority_encoder.Encode(encoder_input);
    if (match && last > match_pos) {
      EXPECT_EQ(priority_encoder.GetValidPositionPair().valid, 1) << "i: " << i << ", last: " << last;
      EXPECT_EQ(priority_encoder.GetValidPositionPair().pos, match_pos) << "i: " << i << ", last: " << last;
    } else {
      EXPECT_EQ(priority_encoder.GetValidPositionPair().valid, 0) << "i: " << i << ", last: " << last;
      EXPECT_EQ(priority_encoder.GetValidPositionPair().pos, -1) << "i: " << i << ", last: " << last;
    }
  }

  // Test if module output correctly matches
  if (match) {
    ASSERT_EQ(priority_encoder.GetValidPositionPair().valid, 1);
    ASSERT_EQ(priority_encoder.GetValidPositionPair().pos, match_pos);
  } else {
    ASSERT_EQ(priority_encoder.GetValidPositionPair().valid, 0);
    ASSERT_EQ(priority_encoder.GetValidPositionPair().pos, -1);
  }

  // Test Reset
  priority_encoder.Reset();
  EXPECT_EQ(priority_encoder.GetValidPositionPair().valid, 0);
  EXPECT_EQ(priority_encoder.GetValidPositionPair().pos, -1);
  EXPECT_EQ(priority_encoder.GetPosition(), 0);
}
