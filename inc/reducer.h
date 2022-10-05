#ifndef __REDUCER_H__
#define __REDUCER_H__

#include <iostream>
#include "specs.h"

class Reducer {
  public:
    Reducer(){};
    Reducer(int num_of_PE_set);
 
    bool output_ready; // true if there are oa bundle ready to output
    bool finish; // true if entire feature map has been compressed into bundle set and finish output

    // need to set conv config before each conv operation
    void SetConvConfig(specs::ConvConfig, std::vector<int>);

    // input partial sum, ia bundle index (index indicating which ia bundle is being calculated), 
    void Reduce(specs::RawData, int ia_h, int ia_w);

    // if PE finish all partial sum processing, call this function
    void AllPartialSumFinish();

    // if Reducer.output_ready, call this function to receive output oa (not compressed yet)
    std::vector<int> GetOA(int&, int&);

    // for debug use, print buffer details
    void PrintBuffer(bool, std::ostream& = std::cout);
    void PrintReducerInfo(std::ostream& = std::cout);
  
  private:
    // reducer config
    int num_of_PE_set_; // to determine the size of partial sum buffer

    // convolution config
    specs::ConvConfig conv_config_; // config for output activations bundle set
    std::vector<int> bias_;

    // the smallest bundle index currently processing in PE
    // update by Reduce()
    int ia_bundle_h_processing_in_PE_; 
    int ia_bundle_w_processing_in_PE_; 
    // all partial sums for current output feature map have finished calculated
    bool all_partial_sum_finish_;

    // bundle base is used to calculate address in buffer
    // bundle base record the smallest bundle stored in buffer
    int buffer_bundle_base_index_; // smallest bundle's index stored in oa_partial_sum_buffer
    int buffer_bundle_base_h_; // bundle base h
    int buffer_bundle_base_w_; // bundle base w

    // if currently, PE is calculating bundle with index i,
    // then the oa bundle with index (i-offset) can be compressed and sent to SRAM,
    // since all of the partial sum is already finished calculated by PE
    int buffer_bundle_finish_offset_; // (kernel-1)xoa_width+kernel + one batch input offset

    // buffer
    specs::RawData oa_partial_sum_buffer_; // output activations(partial sum) buffer, need big buffer

    // output
    int oa_bundle_w_; // output bundle w
    int oa_bundle_h_; // output bundle h
    std::vector<int> oa_output_buffer_; // output oa buffer
    int oa_h_output_buffer_;
    int oa_w_output_buffer_;

    void PostProcess();
    void InitBuffer();
    int  BufferAddressCounter(int, int, int); // given partial sum address, return buffer index
    void MoveBundleBase();
    void UpdateOutputReady();
    void ResetBuffer();
};





#endif // __REDUCER_H__