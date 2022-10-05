//
// TODO(Ray02250418): Add copyright message
//

#ifndef __2022_SPRING_CMODEL_INC_SPECS_H__
#define __2022_SPRING_CMODEL_INC_SPECS_H__

#include <vector>

namespace specs {
constexpr int kInputFeatureMap_H {128};
constexpr int kInputFeatureMap_W {128};
constexpr int kComparatorArraySize {32};
constexpr int kPriorityEncoderNumPerAIM {kComparatorArraySize};
constexpr int kIaBundleMaxSize {256};
constexpr int kWBundleMaxSize {256};
constexpr int kChannelBitWidth {8};
constexpr int kDataBitWidth {8};

// constexpr int kDramSize {500*1024*1024*8}; //500MB
// constexpr int kSramSize {400*1024*8}; //400KB

struct RawData {
  std::vector<int> h;
  std::vector<int> w;
  std::vector<int> k;
  std::vector<int> value; // set int8_t to int, cuz in reducer, it'll overflow
};

struct InputActivationBundle {
  std::vector<int> data;
  std::vector<int> channel_idx; //uint8_t
  int h;
  int w;
  int depth;
};
/*
  one ia bundle  bit
  127x127x3

  48 bit

  a[0:8] = h
  a[9:17] = w
  a[18:19] = channel_idx
  a[20:27] = data
  a[28:29] = channel_idx
  a[30:37] = data
  a[38:39] = channel_idx
  a[40:47] = data

  12x12x192
  a[0:8] = h
  a[9:17] = w
  a[18:25] = channel_idx
  a[26:33] = data
  ...
  x192
*/

// one bundle 2kB, addr 10k

struct InputActivationBundleSet {
  std::vector<InputActivationBundle> input_activation_bundles;
  // record how many data will be record in one bundle
  // for example, if Input Activation size: 127x127x3, then depth_of_bundle = 3
  int H;
  int W;
  int depth_of_bundle; // ia_channel
};

struct WeightBundle{
  std::vector<int> data;
  std::vector<int> channel_idx;
  std::vector<int> pos_ptr;
  std::vector<int> r_idx;
  std::vector<int> k_idx;
  int s;
  int depth;
  int prk_depth;
};

struct WeightBundleSet {
  std::vector<WeightBundle> weight_bundles;
  int R;
  int K;
};

struct ValidPositionPair {
  int valid;
  int pos;
};
// struct ValidPositionPairs {
//   std::vector<ValidPositionPair> valid_position_pairs;
// };

struct ConvConfig {
  bool relu;
  std::vector<int> bias;
  int kernel_size;     // kernel size
  int stride;
  int ia_size; // ia's h, w
  int ia_channel; // input channel

  int oa_size;
  int oa_channel;

  // w & ia bundle set index should be mutually unique
  // since they're both stored in DRAM
  int w_bundle_set_index;  // give each weight bundle set an index
  int ia_bundle_set_index; // give each ia bundle set an index
  int oa_bundle_set_index; // give each ia bundle set an index
  int num_of_w_bundle;    // how much weight bundles are in this conv's weight bundle set
  int num_of_ia_bundle;   // how much ia bundles are in this conv's IA bundle set
  int num_of_oa_bundle;
  
  int w_bundle_set_dram_addr;  // addr for w bundle set saved in DRAM
  int ia_bundle_set_dram_addr; // addr for ia bundle set saved in DRAM

  bool max_pool;

  int quantize_M;
  int quantize_n;
};

struct MaxPoolConfig {
  int  max_pool_kernel_size;
  int  max_pool_stride;

  int ia_size;
  int ia_channel;
  int oa_size;
  int oa_channel;
  int num_of_ia_bundle;
  int num_of_oa_bundle;

  int ia_bundle_set_index;
  int oa_bundle_set_index;
};

}  // namespace specs

// backbone alexnet
extern specs::ConvConfig    ALEXNET_IMGZ_CONV1;
extern specs::ConvConfig    ALEXNET_IMGZ_CONV2;
extern specs::MaxPoolConfig ALEXNET_IMGZ_MAXPOOL1;
extern specs::ConvConfig    ALEXNET_IMGZ_CONV3;
extern specs::MaxPoolConfig ALEXNET_IMGZ_MAXPOOL2;
extern specs::ConvConfig    ALEXNET_IMGZ_CONV4;
extern specs::ConvConfig    ALEXNET_IMGZ_CONV5;
extern specs::ConvConfig    ALEXNET_IMGZ_CONV6;

extern specs::ConvConfig    ALEXNET_IMGX_CONV1;
extern specs::ConvConfig    ALEXNET_IMGX_CONV2;
extern specs::MaxPoolConfig ALEXNET_IMGX_MAXPOOL1;
extern specs::ConvConfig    ALEXNET_IMGX_CONV3;
extern specs::MaxPoolConfig ALEXNET_IMGX_MAXPOOL2;
extern specs::ConvConfig    ALEXNET_IMGX_CONV4;
extern specs::ConvConfig    ALEXNET_IMGX_CONV5;
extern specs::ConvConfig    ALEXNET_IMGX_CONV6;

// init
extern specs::ConvConfig C_Z_K;
extern specs::ConvConfig R_Z_K;

extern specs::ConvConfig C_Z;
extern specs::ConvConfig R_Z;


// track
// cls & ctr branch
extern specs::ConvConfig C_X;
extern specs::ConvConfig C_CONV1;
extern specs::ConvConfig C_CONV2;
extern specs::ConvConfig C_CONV3;
extern specs::ConvConfig C_CLS_CONV;
extern specs::ConvConfig C_CTR_CONV;

// reg branch
extern specs::ConvConfig R_X;
extern specs::ConvConfig R_CONV1;
extern specs::ConvConfig R_CONV2;
extern specs::ConvConfig R_CONV3;
extern specs::ConvConfig R_REG_CONV;

extern specs::ConvConfig TEST_CONV;
extern specs::ConvConfig TEST_CONV_S2;

#endif  // __2022_SPRING_CMODEL_INC_SPECS_H__
