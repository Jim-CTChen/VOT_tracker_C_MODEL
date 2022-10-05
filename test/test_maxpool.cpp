#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <assert.h>
#include <stdlib.h>

#include "../inc/max_pool.h"
#include "../inc/specs.h"

using namespace std;

void PrintIABundle(specs::InputActivationBundle b, ostream& stream = cout) {
  assert(b.data.size() == b.depth);
  assert(b.channel_idx.size() == b.depth);

  stream << "===== bundle info =====" << endl;
  stream << "\t" << "(h, w): (" << b.h  << ", " << b.w << ")" << endl;
  stream << "\t" << "size: " << b.depth << endl;
  for (int i = 0; i < b.depth; i++) {
    stream << "\t" << "<channel, data>: <" << b.channel_idx[i] << ", " << b.data[i] << ">" << endl;
  }
  stream << "======================" << endl;
}

class TestMaxPool {
  public:
    TestMaxPool();
    void SetConvConfig(specs::ConvConfig, specs::MaxPoolConfig);
    void GenerateFeatureMapAndGolden();
    void TestOneMaxPoolLayer(specs::ConvConfig, specs::MaxPoolConfig, ostream& stream);
    void Test(ostream&);
    void PrintFeatureMap(int, int, ostream&);
  private:
    bool ValidateOutput(int, int, specs::InputActivationBundle);
    void ResetFeatureMapAndGolden();
    void CompressIABundle(int, int);
    int MaxPooling(int, int, int);

    specs::ConvConfig conv_config_;
    specs::MaxPoolConfig max_pool_config_;
    vector<vector<vector<int>>> raw_feature_map_;
    vector<vector<specs::InputActivationBundle>> feature_map_; // in bundle format
    vector<vector<specs::InputActivationBundle>> golden_;
    MaxPool my_maxpool_;
};

TestMaxPool::TestMaxPool() {}

void TestMaxPool::SetConvConfig(specs::ConvConfig config, specs::MaxPoolConfig m_config) {
  this->conv_config_ = config;
  this->max_pool_config_ = m_config;
  this->my_maxpool_.SetConfig(m_config);
  this->ResetFeatureMapAndGolden();
}

void TestMaxPool::GenerateFeatureMapAndGolden() {
  // random generate raw feature map
  int w = this->conv_config_.oa_size;
  int channel = this->conv_config_.oa_channel;
  for (int i = 0; i < w*w*channel/2; i++) {
    int random_h = rand() % w;
    int random_w = rand() % w;
    int random_k = rand() % channel;
    int random_value = (rand() % 256) - 128;
    this->raw_feature_map_[random_h][random_w][random_k] = random_value;
  }
  // convert into bundle format 
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < w; j++) {
      specs::InputActivationBundle oa_bundle;
      oa_bundle.h = i;
      oa_bundle.w = j;
      int size = 0;
      for (int c = 0; c < channel; c++) {
        if (this->raw_feature_map_[i][j][c] != 0) {
          oa_bundle.channel_idx.push_back(c);
          oa_bundle.data.push_back(this->raw_feature_map_[i][j][c]);
          size ++;
        }
      }
      oa_bundle.depth = size;
      this->feature_map_[i][j] = oa_bundle;
    }
  }

  // calculate golden
  for (int y = 0; y < this->max_pool_config_.oa_size; y++) {
    for (int x = 0; x < this->max_pool_config_.oa_size; x++) {
      specs::InputActivationBundle oa_bundle;
      oa_bundle.h = y;
      oa_bundle.w = x;
      int size = 0;
      int h = y*2+1; // location in feature map
      int w = x*2+1; // location in feature map
      for (int c = 0; c < channel; c++) {
        int max = this->MaxPooling(h, w, c);
        if (max != 0) {
          oa_bundle.channel_idx.push_back(c);
          oa_bundle.data.push_back(max);
          size += 1;
        }
      }
      oa_bundle.depth = size;
      this->golden_[y][x] = oa_bundle;
    }
  }
}

void TestMaxPool::TestOneMaxPoolLayer(specs::ConvConfig config, specs::MaxPoolConfig m_config, ostream& stream) {
  this->SetConvConfig(config, m_config);
  this->GenerateFeatureMapAndGolden();
  this->Test(stream);
}

void TestMaxPool::Test(ostream& stream) {
  // ostream& error_stream = stream;
  ostream& error_stream = cerr;
  int finish_bundle_index = 0;
  int h = 0;
  int w = 0;
  int end = this->conv_config_.num_of_oa_bundle-1;
  bool has_error = false;
  while (finish_bundle_index != end) {
    // set bundle index
    this->my_maxpool_.SetReducerIABundleIndex(finish_bundle_index);
    stream << "sending ia bundle index: " << finish_bundle_index << endl;
    this->my_maxpool_.PrintStatus(stream);

    // send bundles in
    bool should_have_output = this->my_maxpool_.request; // if there's request, should have output
    if (this->my_maxpool_.request) stream << "requesting ia bundles..." << endl;
    else stream << "not requesting ia bundles..." << endl;
    while (this->my_maxpool_.request) {
      // requesting oa bundle
      int requested_bundle_index = this->my_maxpool_.request_bundle_index;
      int h = requested_bundle_index / this->conv_config_.oa_size;
      int w = requested_bundle_index % this->conv_config_.oa_size;
      this->my_maxpool_.SendBundle(this->feature_map_[h][w]);
      stream << "sending bundles (h, w) = (" << h << ", " << w << ")" << endl;
      PrintIABundle(this->feature_map_[h][w], stream);
      this->my_maxpool_.PrintStatus(stream);
    }
    
    // check for output ready
    if (should_have_output && !this->my_maxpool_.output_ready) {
      error_stream << "Something wrong! No output!" << endl;
      error_stream << "Current bundle index: " << finish_bundle_index << endl;
    }
    
    // validate output
    if (should_have_output) {
      stream << "Expecting output bundle at (h, w) = (" << h << ", " << w << ")" << endl;
      specs::InputActivationBundle oa_bundle = this->my_maxpool_.GetOABundle();
      if (!this->ValidateOutput(h, w, oa_bundle)) {
        has_error = true;
        error_stream << "Error occur at (h, w) = (" << h << ", " << w << ")" << endl;
        error_stream << "Receive bundle: " << endl;
        PrintIABundle(oa_bundle, error_stream);
        error_stream << "Golden: " << endl;
        PrintIABundle(this->golden_[h][w]);
        // error_stream << "Raw: " << endl;
        // this->PrintFeatureMap(h, w, error_stream);
      }
      else {
        stream << "Output bundle correct!" << endl;
      }
      if (w == this->max_pool_config_.oa_size-1) {
        if (h == this->max_pool_config_.oa_size-1) {
          w = 0;
          h = 0;
        }
        else {
          w = 0;
          h += 1;
        }
      }
      else {
        w += 1;
      }
    }
    stream << "end iteration" << endl;

    // move bundle index
    finish_bundle_index ++;
  }
  if (!has_error) cerr << "Congrats!!! No error!!!" << endl;
}

bool TestMaxPool::ValidateOutput(int h, int w, specs::InputActivationBundle oa_bundle) {
  if (h != oa_bundle.h) return false;
  if (w != oa_bundle.w) return false;
  if (this->golden_[h][w].channel_idx != oa_bundle.channel_idx) return false;
  if (this->golden_[h][w].data != oa_bundle.data) return false;
  return true;
} 

void TestMaxPool::ResetFeatureMapAndGolden() {
  // reset feature map
  this->feature_map_.clear();
  int h = this->conv_config_.oa_size;
  this->feature_map_ = vector<vector<specs::InputActivationBundle>> (h, vector<specs::InputActivationBundle>(h));
  
  // reset raw feature map
  int k = this->conv_config_.oa_channel;
  this->raw_feature_map_ = vector<vector<vector<int>>> (h, vector<vector<int>>(h, vector<int>(k)));

  // reset golden
  this->golden_.clear();
  int x = this->max_pool_config_.oa_size;
  this->golden_ = vector<vector<specs::InputActivationBundle>> (x, vector<specs::InputActivationBundle>(x));
}

void TestMaxPool::PrintFeatureMap(int h, int w, ostream& stream) {
  stream << "Printing feature map at (h, w) = (" << h << ", " << w << ")" << endl;
  for (int c = 0; c < this->conv_config_.oa_channel; c++) {
    stream << "(c, value) = (" << c << ", " << this->raw_feature_map_[h][w][c] << ")" << endl;
  } 
}

void TestMaxPool::CompressIABundle(int h, int w) {

}

int TestMaxPool::MaxPooling (int h, int w, int k) {
  // use ceil mode, pad right & bottom edge cases
  bool right_edge = (w == this->conv_config_.oa_size-1);
  bool bottom_edge = (h == this->conv_config_.oa_size-1);
  int v[9];
  v[0] = this->raw_feature_map_[h-1][w-1][k];
  v[1] = this->raw_feature_map_[h-1][w][k];
  v[3] = this->raw_feature_map_[h][w-1][k];
  v[4] = this->raw_feature_map_[h][w][k];
  if (right_edge) {
    v[2] = this->raw_feature_map_[h][w][k];
    v[5] = this->raw_feature_map_[h][w][k];
  }
  else {
    v[2] = this->raw_feature_map_[h-1][w+1][k];
    v[5] = this->raw_feature_map_[h][w+1][k];
  }
  if (bottom_edge) {
    v[6] = this->raw_feature_map_[h][w][k];
    v[7] = this->raw_feature_map_[h][w][k];
  }
  else {
    v[6] = this->raw_feature_map_[h+1][w-1][k];
    v[7] = this->raw_feature_map_[h+1][w][k];
  }
  if (right_edge || bottom_edge) {
    v[8] = this->raw_feature_map_[h][w][k];
  }
  else {
    v[8] = this->raw_feature_map_[h+1][w+1][k];
  }

  int max = -128;
  for (int i = 0; i < 9; i++) {
    if (v[i] > max) max = v[i];
  }
  return max;
}

specs::ConvConfig CONV_1_CONFIG;
specs::ConvConfig CONV_2_CONFIG;
specs::ConvConfig CONV_3_CONFIG;
specs::ConvConfig CONV_4_CONFIG;
specs::ConvConfig CONV_5_CONFIG;
specs::MaxPoolConfig MAX_POOL_1_CONFIG;
specs::MaxPoolConfig MAX_POOL_2_CONFIG;
specs::MaxPoolConfig MAX_POOL_3_CONFIG;
specs::MaxPoolConfig MAX_POOL_4_CONFIG;
specs::MaxPoolConfig MAX_POOL_5_CONFIG;

void InitConvConfig() {

  // normal 3x3 kernel
  // CONV_1_CONFIG.bias = -1;
  CONV_1_CONFIG.relu = true;
  CONV_1_CONFIG.stride = 1;
  CONV_1_CONFIG.oa_size = 17;
  CONV_1_CONFIG.oa_channel = 5;
  CONV_1_CONFIG.kernel_size = 3;
  CONV_1_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_1_CONFIG.num_of_oa_bundle = 17*17;

  MAX_POOL_1_CONFIG.ia_size = 17;
  MAX_POOL_1_CONFIG.ia_channel = 8;
  MAX_POOL_1_CONFIG.oa_size = 8;
  MAX_POOL_1_CONFIG.oa_channel = 5;
  MAX_POOL_1_CONFIG.max_pool_kernel_size = 3;
  MAX_POOL_1_CONFIG.max_pool_stride = 2;

  // normal 5x5 kernel
  // CONV_2_CONFIG.bias = 100;
  CONV_2_CONFIG.relu = true;
  CONV_2_CONFIG.stride = 1;
  CONV_2_CONFIG.oa_size = 7;
  CONV_2_CONFIG.oa_channel = 3;
  CONV_2_CONFIG.kernel_size = 3;
  CONV_2_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_2_CONFIG.num_of_oa_bundle = 7*7;

  
  MAX_POOL_2_CONFIG.ia_size = 7;
  MAX_POOL_2_CONFIG.ia_channel = 7;
  MAX_POOL_2_CONFIG.oa_size = 3;
  MAX_POOL_2_CONFIG.oa_channel = 3;
  MAX_POOL_2_CONFIG.max_pool_kernel_size = 3;
  MAX_POOL_2_CONFIG.max_pool_stride = 2;

  // normal 4x4 kernel
  // CONV_3_CONFIG.bias = 20;
  CONV_3_CONFIG.relu = true;
  CONV_3_CONFIG.stride = 1;
  CONV_3_CONFIG.oa_size = 149;
  CONV_3_CONFIG.oa_channel = 128;
  CONV_3_CONFIG.kernel_size = 3;
  CONV_3_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_3_CONFIG.num_of_oa_bundle = 149*149;

  MAX_POOL_3_CONFIG.oa_size = 74;
  MAX_POOL_3_CONFIG.max_pool_kernel_size = 3;
  MAX_POOL_3_CONFIG.max_pool_stride = 2;

  // no relu 3x3 kernelma
  // CONV_4_CONFIG.bias = -15;
  CONV_4_CONFIG.relu = false;
  CONV_4_CONFIG.stride = 1;
  CONV_4_CONFIG.oa_size = 8;
  CONV_4_CONFIG.oa_channel = 3;
  CONV_4_CONFIG.kernel_size = 3;
  CONV_4_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_4_CONFIG.num_of_oa_bundle = 8*8;

  MAX_POOL_4_CONFIG.oa_size = 4;
  MAX_POOL_4_CONFIG.max_pool_kernel_size = 3;
  MAX_POOL_4_CONFIG.max_pool_stride = 2;

  // big feature map
  // CONV_5_CONFIG.bias = 102;
  CONV_5_CONFIG.relu = false;
  CONV_5_CONFIG.stride = 1;
  CONV_5_CONFIG.oa_size = 59;
  CONV_5_CONFIG.oa_channel = 128;
  CONV_5_CONFIG.kernel_size = 3;
  CONV_5_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_5_CONFIG.num_of_oa_bundle = 59*59;



}

int main() {
  std::string log_file_path = "logfile.log";
  ofstream myfile;
  myfile.open(log_file_path);
  ostream& stream = myfile;

  InitConvConfig();

  TestMaxPool tb;

  tb.TestOneMaxPoolLayer(CONV_1_CONFIG, MAX_POOL_1_CONFIG, stream);
  tb.TestOneMaxPoolLayer(CONV_2_CONFIG, MAX_POOL_2_CONFIG, stream);
  // tb.TestOneMaxPoolLayer(CONV_3_CONFIG, MAX_POOL_3_CONFIG, stream);
  // tb.TestOneMaxPoolLayer(CONV_4_CONFIG, MAX_POOL_4_CONFIG, stream);
}