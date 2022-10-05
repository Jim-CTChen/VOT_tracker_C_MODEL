#ifndef _2022_SPRING_CMODEL_INC_PE_SEQUENCE_DECODER_H_
#define _2022_SPRING_CMODEL_INC_PE_SEQUENCE_DECODER_H_

#include <vector>

#include "specs.h"

class SequenceDecoder{
  public:
    SequenceDecoder();

    // input valid position pairs
    void InputValPos(std::vector<specs::ValidPositionPair>);

    // decode w_addr & ia_addr
    void Decode();

    // return w_addr, ia_addr
    int GetAddr(std::vector<int>&, std::vector<int>&);

    // all valid position pair finish decode
    bool finish_decoding;
    
    void Reset();
  private:
    std::vector<specs::ValidPositionPair> val_pos_pairs_;
    int val_pos_ptr_;       // pointer for valid position pairs
    int invalid_offset_;     // number of invalid in valid position pairs
    int num_of_valid_addr;  // number of valid address decoded by decoder in this batch, range from [0, 3]
    std::vector<int> w_addr_;
    std::vector<int> ia_addr_;
};



#endif // _2022_SPRING_CMODEL_INC_PE_SEQUENCE_DECODER_H_