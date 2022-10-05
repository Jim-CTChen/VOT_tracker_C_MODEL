//
// TODO(Ray02250418): Add copyright message
//

#ifndef _2022_SPRING_CMODEL_TEST_UTILS_H_
#define _2022_SPRING_CMODEL_TEST_UTILS_H_

#include <stdlib.h>
#include <time.h>

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

namespace utils {
unsigned int seed = (unsigned int)time(NULL);

struct Gen {
  int range;
  explicit Gen(int range_): range(range_) {}
  int operator()() {
    if (range <= 0 || range > RAND_MAX) {
      return rand_r(&seed);
    }
    return rand_r(&seed) % (range * 2 + 1) - range;
  }
};

std::vector<int> GetRandomIntVector(int min_length, int max_length, int range, bool pos_increasing = false) {
  int length = rand_r(&seed) % (max_length - min_length + 1) + min_length;
  if (pos_increasing) {
    std::vector<int> vec(range);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
    std::sort(vec.begin(), vec.end());
    vec.resize(length);
    return vec;
  } else {
    std::vector<int> vec(length);
    Gen gen(range);
    std::generate(vec.begin(), vec.end(), gen);
    return vec;
  }
}

std::vector<int> GetRandomIntVector(int length, int range, bool pos_increasing = false) {
  if (pos_increasing) {
    std::vector<int> vec(range);
    std::iota(vec.begin(), vec.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
    std::sort(vec.begin(), vec.end());
    vec.resize(length);
    return vec;
  } else {
    std::vector<int> vec(length);
    Gen gen(range);
    std::generate(vec.begin(), vec.end(), gen);
    return vec;
  }
}
}  // namespace utils

#endif  // _2022_SPRING_CMODEL_TEST_UTILS_H_