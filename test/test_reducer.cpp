#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include "../inc/reducer.h"
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

void PrintPartialSum(specs::RawData raw, ostream& stream = cout) {
  int h, w, k, v;
  for (int i = 0; i < raw.h.size(); i++) {
    h = raw.h[i];
    w = raw.w[i];
    k = raw.k[i];
    v = raw.value[i];
    stream << "(h, w, k, value): " << "(" << h << ", " << w << ", " << k << ", " << v << ")" << endl;
  }
}

class TestReducer {
  public:
    TestReducer(int num_of_PE_set);

    // test pipeline
    void SetConvConfig(specs::ConvConfig);
    void GenerateBatch(int, ostream&, bool); // PE still working on same IA bundle
    void NextIABundle(ostream&, bool);  // PE finish current IA bundle, move to next location
    void RecieveOutput(ostream&, bool);

    // debug use
    void PrintGolden(ostream&, int, int, vector<int>&);
    void PrintReducerBuffer(bool with_info, ostream&);
    void Validation(ostream&);
    void GenGolden();

    vector<vector<vector<int>>> partial_sum;
    vector<vector<specs::InputActivationBundle>> golden;
    vector<vector<specs::InputActivationBundle>> my_ans;

    Reducer my_reducer = 0;
    bool pe_finish;
    bool has_error;

  private:
    specs::RawData RandomGeneratePartialSum(int start_idx, int end_idx, int n);
    void AddPartialSum(specs::RawData & ps, int h, int w, int k, int value);
    bool ValidateOneBundle(int, int);
    void InitMyAnswer();
    specs::ConvConfig conv_config_;
    int num_of_PE_set_;
    int ia_bundle_index_; // set to smallest bundle which is proccessing
    int bundle_count_;
    int partial_sum_size_;
    int oa_h_;
    int oa_w_;
};

TestReducer::TestReducer(int num_of_PE_set) {
  this->num_of_PE_set_ = num_of_PE_set;
  this->ia_bundle_index_ = 0;
  this->bundle_count_ = 0;
  this->pe_finish = false;
  this->my_reducer = Reducer(num_of_PE_set);
  this->oa_h_ = 0;
  this->oa_w_ = 0;
}

void TestReducer::SetConvConfig(specs::ConvConfig config) {
  this->conv_config_ = config;
  int h = config.oa_size;
  int k = config.oa_channel;
  int kernel = config.kernel_size;
  int stride = config.stride;
  this->partial_sum_size_ = (config.ia_size-kernel+1);
  this->golden.clear();
  this->InitMyAnswer();
  this->partial_sum.clear();
  this->partial_sum = vector<vector<vector<int>>> (this->partial_sum_size_, vector<vector<int>>(this->partial_sum_size_, vector<int>(k)));

  this->my_reducer.SetConvConfig(config);

  this->ia_bundle_index_ = 0;
  this->bundle_count_ = 0;
  this->pe_finish = false;
  this->oa_h_ = 0;
  this->oa_w_ = 0;


  this->has_error = false;
}

void TestReducer::RecieveOutput(ostream& stream, bool verbose) {
  ostream& error_stream = cerr;
  vector<int> ground_truth;
  while (my_reducer.output_ready) {
    this->my_reducer.PrintBuffer(false, stream);
    this->bundle_count_ += 1;
    specs::InputActivationBundle oa_bundle = my_reducer.GetOABundle();
    if (verbose) {
      stream << "Bundle " << this->bundle_count_ << endl;
      stream << "Output ready, receiving bundle (h, w) = (" << oa_bundle.h << ", " << oa_bundle.w << ") from reducer..." << endl;
      // PrintIABundle(oa_bundle, stream);
    }

    // check if output bundle in order
    if (oa_bundle.h != this->oa_h_ || oa_bundle.w != this->oa_w_) {
      cerr << "Order Error!! Expecting oa bundle at (h, w) == (" << this->oa_h_ << ", " << this->oa_w_ << ")" << endl;
      cerr << "Receive bundle at (h, w) == (" << oa_bundle.h << ", " << oa_bundle.w << ")" << endl;
      this->has_error = true;
    }
    this->my_ans[oa_bundle.h][oa_bundle.w] = oa_bundle;

    // move (h, w)
    if (this->oa_w_ == this->conv_config_.oa_size-1) {
      if (this->oa_h_ == this->conv_config_.oa_size-1) {
        this->oa_h_ = 0;
        this->oa_w_ = 0;
      }
      else {
        this->oa_h_++;
        this->oa_w_ = 0;
      }
    }
    else this->oa_w_++;
  }
}

void TestReducer::GenerateBatch(int n, ostream& stream, bool verbose) {
  if (this->pe_finish) return;

  // Generating batch of partial sums...
  int ia_bundle_index = this->ia_bundle_index_;
  int kernel = this->conv_config_.kernel_size;
  int start_idx = ia_bundle_index;
  int end_idx = ia_bundle_index + this->num_of_PE_set_;

  if (end_idx > this->conv_config_.num_of_ia_bundle) {
    end_idx = this->conv_config_.num_of_ia_bundle;
  }
  int h_start = start_idx/this->conv_config_.ia_size;
  int w_start = start_idx%this->conv_config_.ia_size;
  int h_end = end_idx/this->conv_config_.ia_size;
  int w_end = end_idx%this->conv_config_.ia_size;
  stream << "Generating Partial Sum from (" << h_start << ", " << w_start << ")" << " to (";
  stream << h_end << ", " << w_end << ")" << endl;
  specs::RawData partial_sum = RandomGeneratePartialSum(start_idx, end_idx, n);
  // if (verbose) {
  // PrintPartialSum(partial_sum, stream);
  // }


  // reducer reduce
  my_reducer.Reduce(partial_sum, h_start, w_start);
  if (verbose) my_reducer.PrintBuffer(false, stream);

  // check output
  this->RecieveOutput(stream, verbose);

  if (verbose) my_reducer.PrintBuffer(false, stream);
}

void TestReducer::NextIABundle(ostream& stream, bool verbose) {
  // if (verbose) {
  //   cout << "Moving ia bundle index..." << endl;
  // }
  // if bundle index reach end, all partial sums had been calculated
  if (this->ia_bundle_index_+this->num_of_PE_set_ >= this->conv_config_.num_of_oa_bundle) {
    // end of PE processing
    // ia bundle index indicates the smallest bundle index processing in PEs
    this->ia_bundle_index_ = this->conv_config_.num_of_oa_bundle-1;
    this->pe_finish = true;
    this->my_reducer.AllPartialSumFinish();
    this->RecieveOutput(stream, verbose);
    if (verbose) my_reducer.PrintBuffer(false, stream);
  }

  // hasn't reach end, move bundle index
  else this->ia_bundle_index_ += this->num_of_PE_set_;
}

void TestReducer::GenGolden() {
  int stride = this->conv_config_.stride;
  this->golden.clear();

  for (int h = 0; h < this->conv_config_.oa_size; h++) {
    vector<specs::InputActivationBundle> row;
    for (int w = 0; w < this->conv_config_.oa_size; w++) {
      int ps_h = h*stride;
      int ps_w = w*stride; // (h, w) in partial sum's shape
      specs::InputActivationBundle oa_bundle;
      oa_bundle.h = h;
      oa_bundle.w = w;
      int size = 0;
      for (int c = 0; c < this->conv_config_.oa_channel; c++) {
        int value = this->partial_sum[ps_h][ps_w][c] + this->conv_config_.bias[c];
        value = ((value*this->conv_config_.quantize_M) >> this->conv_config_.quantize_n);
        if (this->conv_config_.relu && (value < 0)) value = 0;
        if (value != 0) {
          size++;
          oa_bundle.channel_idx.push_back(c);
          oa_bundle.data.push_back(value);
        }
      }
      oa_bundle.depth = size;
      row.push_back(oa_bundle);
    }
    this->golden.push_back(row);
  }
}

void TestReducer::Validation(ostream& stream) {
  // check bundles
  for (int h = 0; h < this->conv_config_.oa_size; h++) {
    for (int w = 0; w < this->conv_config_.oa_size; w++) {
      stream << "Validating oa bundle at (h, w) = (" << h << ", " << w << ")" << endl;
      if (!this->ValidateOneBundle(h, w)) {
        cerr << "Value Error!!" << endl;
        cerr << "Golden:" << endl;
        PrintIABundle(this->golden[h][w], cerr);
        cerr << "My answer:" << endl;
        PrintIABundle(this->my_ans[h][w], cerr);
        this->has_error = true;
      }
    }
  }
}

bool TestReducer::ValidateOneBundle(int h, int w) {
  specs::InputActivationBundle my_bundle = this->my_ans[h][w];
  specs::InputActivationBundle gt = this->golden[h][w];
  if (my_bundle.depth != gt.depth) return false;
  return (my_bundle.channel_idx == gt.channel_idx) && (my_bundle.data == gt.data);
}

void TestReducer::PrintGolden(ostream& stream, int h, int w, vector<int>& gt) {
  stream << "Golden at (h, w): " << "(" << h << ", " << w << ")" << endl;
  for (auto it : gt) {
    stream << it << " ";
  }
  stream << endl;
}

void TestReducer::PrintReducerBuffer(bool with_info, ostream& stream) {
  this->my_reducer.PrintBuffer(with_info, stream);
}

specs::RawData TestReducer::RandomGeneratePartialSum(int start_idx, int end_idx, int n=10) {
  /*
    randomly generate partial sum,
    value within range -128~127
    location within range [start_idx, end_idx]
    input:
      start_idx: start index, range from [0, num_of_ia_bundle]
      end_idx: end index, range from [start_idx, num_of_ia_bundle]
      n: number of partial sum
    output:
      partial sums
  */
  specs::RawData partial_sum;
  int ia_size = this->conv_config_.ia_size;
  for (int i = 0; i < n; i++) {
    int random_ia = (rand() % (end_idx - start_idx)) + start_idx;
    int h = random_ia / ia_size;
    int w = random_ia % ia_size;
    int r = rand() % this->conv_config_.kernel_size;
    int s = rand() % this->conv_config_.kernel_size;
    h = h-r;
    w = w-s;
    if (h < 0) h = 0;
    if (h > this->partial_sum_size_-1) h = this->partial_sum_size_-1;
    if (w < 0) w = 0;
    if (w > this->partial_sum_size_-1) w = this->partial_sum_size_-1;
    int k = rand() % this->conv_config_.oa_channel;
    int v = (rand() % 256) - 128; // -128 ~ 127
    AddPartialSum(partial_sum, h, w, k, v);
    this->partial_sum[h][w][k] += v;
  }
  return partial_sum;
}

void TestReducer::AddPartialSum(specs::RawData & ps, int h, int w, int k, int value) {
  ps.h.push_back(h);
  ps.w.push_back(w);
  ps.k.push_back(k);
  ps.value.push_back(value);
}

void TestReducer::InitMyAnswer() {
  this->my_ans.clear();
  for (int h = 0; h < this->conv_config_.oa_size; h++) {
    vector<specs::InputActivationBundle> row;
    for (int w = 0; w < this->conv_config_.oa_size; w++) {
      specs::InputActivationBundle dummy;
      dummy.depth = -1;
      row.push_back(dummy);
    }
    this->my_ans.push_back(row);
  }
}

void TestConvolutionLayer(TestReducer tb, int n, specs::ConvConfig config, ostream& stream, bool debug) {
  bool has_error = false;
  stream << "----------------- Convolution layer " << n << " -----------------" << endl;
  tb.SetConvConfig(config);
  bool print_buffer_info = true;
  tb.PrintReducerBuffer(print_buffer_info, stream);

  int num_of_partial_sum = config.num_of_oa_bundle;

  stream << "Sending first partial sum..." << endl;
  stream << "Expect no output bundle" << endl;
  tb.GenerateBatch(num_of_partial_sum, stream, debug);
  tb.NextIABundle(stream, debug);
  tb.PrintReducerBuffer(false, stream);

  stream << "Sending all partial sum..." << endl;
  while (!tb.pe_finish) {
    tb.GenerateBatch(num_of_partial_sum, stream, debug);
    tb.NextIABundle(stream, debug);
    tb.PrintReducerBuffer(false, stream);
  }
  if (!tb.my_reducer.finish) {
    cerr << "Error!! Reducer should finished processing!" << endl;
  }
  else stream << "Reducer finish processing" << endl;
  tb.GenGolden();
  stream << "Start Validation" << endl;
  tb.Validation(stream);
  if (!tb.has_error) cerr << "No error occur at layer " << n << "!!" << endl;
  stream << "Finish Convolution layer " << n << "!!!" << endl;
}

specs::ConvConfig CONV_1_CONFIG;
specs::ConvConfig CONV_2_CONFIG;
specs::ConvConfig CONV_3_CONFIG;
specs::ConvConfig CONV_4_CONFIG;
specs::ConvConfig CONV_5_CONFIG;
specs::ConvConfig ALEX_CONV1_CONFIG;

void InitConvConfig() {
  // normal 3x3 kernel
  // CONV_1_CONFIG.bias = -1;
  CONV_1_CONFIG.relu = true;
  CONV_1_CONFIG.stride = 1;
  CONV_1_CONFIG.kernel_size = 3;
  CONV_1_CONFIG.ia_size = 9;
  CONV_1_CONFIG.oa_size = (CONV_1_CONFIG.ia_size-CONV_1_CONFIG.kernel_size)/CONV_1_CONFIG.stride+1;
  CONV_1_CONFIG.oa_channel = 3;
  CONV_1_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_1_CONFIG.num_of_oa_bundle = CONV_1_CONFIG.oa_size*CONV_1_CONFIG.oa_size;
  CONV_1_CONFIG.num_of_ia_bundle = CONV_1_CONFIG.ia_size*CONV_1_CONFIG.ia_size;
  for (int i = 0; i < CONV_1_CONFIG.oa_channel; i++) CONV_1_CONFIG.bias.push_back(1);
  CONV_1_CONFIG.quantize_M = 1;
  CONV_1_CONFIG.quantize_n = 1;
  

  // normal 3x3 kernel stide = 2
  // CONV_2_CONFIG.bias = 100;
  CONV_2_CONFIG.relu = true;
  CONV_2_CONFIG.stride = 2;
  CONV_2_CONFIG.kernel_size = 3;
  CONV_2_CONFIG.ia_size = 9;
  CONV_2_CONFIG.oa_size = (CONV_2_CONFIG.ia_size-CONV_2_CONFIG.kernel_size)/CONV_2_CONFIG.stride+1;
  CONV_2_CONFIG.oa_channel = 3;
  CONV_2_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_2_CONFIG.num_of_oa_bundle = CONV_2_CONFIG.oa_size*CONV_2_CONFIG.oa_size;
  CONV_2_CONFIG.num_of_ia_bundle = CONV_2_CONFIG.ia_size*CONV_2_CONFIG.ia_size;
  for (int i = 0; i < CONV_2_CONFIG.oa_channel; i++) CONV_2_CONFIG.bias.push_back(1);
  CONV_2_CONFIG.quantize_M = 2;
  CONV_2_CONFIG.quantize_n = 3;
  
  // normal 4x4 kernel
  // CONV_3_CONFIG.bias = 20;
  CONV_3_CONFIG.relu = true;
  CONV_3_CONFIG.stride = 1;
  CONV_3_CONFIG.kernel_size = 4;
  CONV_3_CONFIG.ia_size = 11;
  CONV_3_CONFIG.oa_size = (CONV_3_CONFIG.ia_size-CONV_3_CONFIG.kernel_size)/CONV_3_CONFIG.stride+1;
  CONV_3_CONFIG.oa_channel = 4;
  CONV_3_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_3_CONFIG.num_of_oa_bundle = CONV_3_CONFIG.oa_size*CONV_3_CONFIG.oa_size;
  CONV_3_CONFIG.num_of_ia_bundle = CONV_3_CONFIG.ia_size*CONV_3_CONFIG.ia_size;
  for (int i = 0; i < CONV_3_CONFIG.oa_channel; i++) CONV_3_CONFIG.bias.push_back(1);
  CONV_3_CONFIG.quantize_M = 5;
  CONV_3_CONFIG.quantize_n = 2;
  
  // no relu 3x3 kernel
  // CONV_4_CONFIG.bias = -15;
  CONV_4_CONFIG.relu = false;
  CONV_4_CONFIG.stride = 1;
  CONV_4_CONFIG.kernel_size = 3;
  CONV_4_CONFIG.ia_size = 10;
  CONV_4_CONFIG.oa_size = (CONV_4_CONFIG.ia_size-CONV_4_CONFIG.kernel_size)/CONV_4_CONFIG.stride+1;
  CONV_4_CONFIG.oa_channel = 5;
  CONV_4_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_4_CONFIG.num_of_oa_bundle = CONV_4_CONFIG.oa_size*CONV_4_CONFIG.oa_size;
  CONV_4_CONFIG.num_of_ia_bundle = CONV_4_CONFIG.ia_size*CONV_4_CONFIG.ia_size;
  for (int i = 0; i < CONV_4_CONFIG.oa_channel; i++) CONV_4_CONFIG.bias.push_back(1);
  CONV_4_CONFIG.quantize_M = 1;
  CONV_4_CONFIG.quantize_n = 0;
  

  // big feature map
  // CONV_5_CONFIG.bias = 102;
  CONV_5_CONFIG.relu = true;
  CONV_5_CONFIG.stride = 1;
  CONV_5_CONFIG.kernel_size = 3;
  CONV_5_CONFIG.ia_size = 151;
  CONV_5_CONFIG.oa_size = (CONV_5_CONFIG.ia_size-CONV_5_CONFIG.kernel_size)/CONV_5_CONFIG.stride+1;
  CONV_5_CONFIG.oa_channel = 96;
  CONV_5_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_5_CONFIG.num_of_oa_bundle = CONV_5_CONFIG.oa_size*CONV_5_CONFIG.oa_size;
  CONV_5_CONFIG.num_of_ia_bundle = CONV_5_CONFIG.ia_size*CONV_5_CONFIG.ia_size;
  for (int i = 0; i < CONV_5_CONFIG.oa_channel; i++) CONV_5_CONFIG.bias.push_back(1);
  CONV_5_CONFIG.quantize_M = 1;
  CONV_5_CONFIG.quantize_n = 0;
  
  // ALEX_CONV1_CONFIG.bias = 102;
  ALEX_CONV1_CONFIG.relu = true;
  ALEX_CONV1_CONFIG.stride = 2;
  ALEX_CONV1_CONFIG.kernel_size = 3;
  ALEX_CONV1_CONFIG.ia_size = 303;
  ALEX_CONV1_CONFIG.oa_size = (ALEX_CONV1_CONFIG.ia_size-ALEX_CONV1_CONFIG.kernel_size)/ALEX_CONV1_CONFIG.stride+1;
  ALEX_CONV1_CONFIG.oa_channel = 96;
  ALEX_CONV1_CONFIG.num_of_w_bundle = 0; // FIXME
  ALEX_CONV1_CONFIG.num_of_oa_bundle = ALEX_CONV1_CONFIG.oa_size*ALEX_CONV1_CONFIG.oa_size;
  ALEX_CONV1_CONFIG.num_of_ia_bundle = ALEX_CONV1_CONFIG.ia_size*ALEX_CONV1_CONFIG.ia_size;
  for (int i = 0; i < ALEX_CONV1_CONFIG.oa_channel; i++) ALEX_CONV1_CONFIG.bias.push_back(1);
  ALEX_CONV1_CONFIG.quantize_M = 1;
  ALEX_CONV1_CONFIG.quantize_n = 0;
  
}


int main() {
  // logging
  std::string log_file_path = "logfile.log";
  ofstream myfile;
  myfile.open(log_file_path);
  // ostream& stream = cout;
  // ostream& stream = cerr;
  ostream& stream = myfile;

  // init convolution config
  InitConvConfig();

  // tb
  stream << "Initing reducer..." << endl;
  int num_of_PE_sets = 9;
  TestReducer tb(num_of_PE_sets);
  bool debug = true;

  /////////////////////////// convolution layer 1 ///////////////////////////
  TestConvolutionLayer(tb, 1, CONV_1_CONFIG, stream, debug);

  /////////////////////////// convolution layer 2 ///////////////////////////
  TestConvolutionLayer(tb, 2, CONV_2_CONFIG, stream, debug);

  /////////////////////////// convolution layer 3 ///////////////////////////
  TestConvolutionLayer(tb, 3, CONV_3_CONFIG, stream, debug);

  /////////////////////////// convolution layer 4 ///////////////////////////
  TestConvolutionLayer(tb, 4, CONV_4_CONFIG, stream, debug);

  /////////////////////////// convolution layer 5 ///////////////////////////
  TestConvolutionLayer(tb, 5, CONV_5_CONFIG, stream, debug);

  /////////////////////////// convolution layer 5 ///////////////////////////
  // TestConvolutionLayer(tb, 6, ALEX_CONV1_CONFIG, stream, debug);

  cout << "Finish Testing!!!" << endl;

  myfile.close();
}