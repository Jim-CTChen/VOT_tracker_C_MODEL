#ifndef __MAX_POOL_H__
#define __MAX_POOL_H__

#include <vector>
#include "specs.h"

class MaxPool {
  // FSM
  static int S_IDLE;
  static int S_IN_CONFIG;
  static int S_CHECK_CAN_OUTPUT;
  static int S_IN_BUNDLE;
  static int S_MAX_POOL;
  static int S_OUTPUT;

  public:
    MaxPool();
    
    // evocate MaxPool module, IDLE -> IN_CONFIG
    void Start(); 

    // set convolution & max pool config
    void SetConfig(specs::MaxPoolConfig);

    // tell max pool module the progress of reducer
    // index means the lastest output oa bundle index
    void SetReducerIABundleIndex(int);

    // send reqested bundle (with request_bundle_index)
    void SendBundle(specs::InputActivationBundle);

    // for debug use, print maxpool info
    void PrintStatus(std::ostream&);
    
    // output signal, output oa bundle
    specs::InputActivationBundle GetOABundle();

    int request_bundle_index; // asking for ia bundle input
    bool request; // true if requesting, request_bundle_index valid
    bool output_ready; // if ready, can call GetOABundle()

  private:
    specs::MaxPoolConfig max_pool_config_;
    void CheckCanMaxPool();
    void UpdateRequest();
    void UpdateOutputReady();
    void UpdateBundleBase();
    void ResetBuffer();
    void MaxPooling();
    int Max(std::vector<int>);
    
    int buffer_bundle_finish_offset_; // (kernel-1)xoa_width+kernel + one batch input offset
    int ia_bundle_index_finish_reduce_; // newest bundle index got compressed and saved in SRAM
    int bundle_base_index_; // index in convolution oa feature map
    int output_bundle_h_; // h in max pool oa feature map
    int output_bundle_w_; // w in max pool oa feature map
    std::vector<specs::InputActivationBundle> bundle_buffer_;
    specs::InputActivationBundle output_oa_bundle_;
    int state_;
    int counter_;
    bool can_maxpool_;


    

};

#endif // __MAX_POOL_H__