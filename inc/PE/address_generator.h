#ifndef _2022_SPRING_CMODEL_INC_PE_ADDRESS_GENERATOR_H_
#define _2022_SPRING_CMODEL_INC_PE_ADDRESS_GENERATOR_H_

#include "specs.h"
#include <vector>

enum ReductionSelect {
  dummy, invalid, ABC, AB, BC, A_B_C
};

class AddressGenerator {
  /*
    Generate x, y, k, reduction selection, partial sum accumulate
  */
  public:
    AddressGenerator();

    // set oa feature map size
    void SetOASizeAndStride(int, int);

    // given h, w, r, s, k, compute addresses
    void ComputeAddr(int num, int h, int w, std::vector<int> r, int s, std::vector<int> k); // check for edge condition

    // output addresses, (x, y, k)
    void GetOAAddr(std::vector<int>&, std::vector<int>&, std::vector<int>&);

    // output reduction select and output valid signal
    // invalid happens when input h, w, r, s, k are invalid or addresses out of oa feature map size
    ReductionSelect SelectReduction(std::vector<bool>&);
    
    void Reset();
  private:
    int oa_size_;
    int stride_;
    std::vector<int> oa_x_; // addresses for OA
    std::vector<int> oa_y_; // addresses for OA
    std::vector<int> oa_k_; // addresses for OA
    ReductionSelect select_reduction_; // select which to reduce, (4+1) cases, 1 for all invalid
    std::vector<bool> output_valid_;
    // valid for three output datapath
    // invalid in two case:
    // 1. input h, w, r, s, k also invalid
    // 2. out of bound (FIXME, should not have to process)
    int last_x_; // last address, for checking accumulate use
    int last_y_; // last address, for checking accumulate use
    int last_k_; // last address, for checking accumulate use
};


#endif // _2022_SPRING_CMODEL_INC_PE_ADDRESS_GENERATOR_H_