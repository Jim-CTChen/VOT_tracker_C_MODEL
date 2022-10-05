#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include <vector>
#include "specs.h"

class Compressor {
  public:
    specs::RawData raw;
    Compressor();
    void SetOAChannel(int);
    void InputRaw(int, int, std::vector<int>);
    specs::InputActivationBundle Compress();
  private:
    int oa_channel_;
    int oa_h_;
    int oa_w_;
    std::vector<int> raw_data_buffer_;
};


#endif // __COMPRESS_H__