#ifndef _2022_SPRING_CMODEL_INC_PE_PROCESSING_ELEMENT_H_
#define _2022_SPRING_CMODEL_INC_PE_PROCESSING_ELEMENT_H_

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

#include "specs.h"
#include "sequence_decoder.h"
#include "address_generator.h"

class PE{
  public:
    PE();

    // set convolution config
    void SetConvConfig(specs::ConvConfig);

    // input ia bundle
    void InputIABundle(specs::InputActivationBundle);

    // input w bundle
    void InputWBundle(specs::WeightBundle);

    // input valid position pairs
    void InputValidPosition(std::vector<specs::ValidPositionPair>);

    // pe process
    void Process(std::ostream& stream = std::cout);

    // return partial sum
    specs::RawData GetPsum(std::ostream& stream = std::cout);

    // debug use
    void PrintFIFO(std::ostream& stream = std::cout);

    // true if there is valid output
    bool output_valid;

    // true if finish processing
    bool finish;
    
  private:
    void UpdateOutputValid();
    void Reset();

    specs::ConvConfig conv_config_;
    specs::InputActivationBundle ia_bundle_buffer_;
    specs::WeightBundle w_bundle_buffer_;
    std::vector<specs::ValidPositionPair> val_pos_pairs;
    SequenceDecoder seq_decoder_;
    AddressGenerator addr_generator_;
    int prk_ptr_;
    std::queue<int> output_value_; // output FIFO
    std::queue<int> output_x_;     // output FIFO
    std::queue<int> output_y_;     // output FIFO
    std::queue<int> output_k_;     // output FIFO
};

#endif  // _2022_SPRING_CMODEL_INC_PE_PROCESSING_ELEMENT_H_
