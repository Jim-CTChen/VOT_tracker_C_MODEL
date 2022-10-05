//
// TODO(Ray02250418): Add copyright message
//

#include "comparator_array.h"

#include <gtest/gtest.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

int rand_() {
  return rand() % 20;
}

TEST(comparator_array, Compare) {
  srand(time(0));
  for (int test_times = 0; test_times < 100; test_times++) {
    int w_channel_idx_length = rand() % specs::kComparatorArraySize + 1;
    int ia_channel_idx_length = rand() % specs::kComparatorArraySize + 1;
    std::vector<int> w_channel_idx(w_channel_idx_length);
    std::vector<int> ia_channel_idx(ia_channel_idx_length);
    std::generate(w_channel_idx.begin(), w_channel_idx.end(), rand_);
    std::generate(ia_channel_idx.begin(), ia_channel_idx.end(), rand_);

    std::vector<std::vector<int>> truth_output;
    std::vector<int> row;
    for (auto i : w_channel_idx) {
      row.clear();
      for (auto j : ia_channel_idx) {
        if (i == j) {
          row.push_back(1);
        } else {
          row.push_back(0);
        }
      }
      truth_output.push_back(row);
    }

    std::vector<std::vector<int>> output;
    ComparatorArray comparator_array;
    output = comparator_array.Compare(w_channel_idx, ia_channel_idx);

    ASSERT_EQ(truth_output.size(), output.size()) << "Output row number diffrent";
    for (int i = 0; i < truth_output.size(); i++) {
      EXPECT_EQ(truth_output.at(i).size(), output.at(i).size());
      for (int j = 0; j < truth_output.at(i).size(); j++) {
        EXPECT_EQ(truth_output.at(i).at(j), output.at(i).at(j)) << "Differ at row " << i << " index " << j;
      }
    }
  }
}
