#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>

#include "specs.h"
#include "PE/address_generator.h"
#include "PE/pe.h"
#include "AIM/associative_index_matching_unit.h"

using namespace std;

void PrintIABundle(specs::InputActivationBundle b, ostream& stream = cout) {
  assert(b.data.size() == b.depth);
  assert(b.channel_idx.size() == b.depth);

  stream << "===== IA bundle info =====" << endl;
  stream << "\t" << "(h, w): (" << b.h  << ", " << b.w << ")" << endl;
  stream << "\t" << "size: " << b.depth << endl;
  for (int i = 0; i < b.depth; i++) {
    stream << "\t" << "<channel, data>: <" << b.channel_idx[i] << ", " << b.data[i] << ">" << endl;
  }
  stream << "======================" << endl;
}

void PrintWBundle(specs::WeightBundle b, ostream& stream = cout) {
  assert(b.data.size() == b.depth);
  assert(b.channel_idx.size() == b.depth);

  stream << "===== W bundle info =====" << endl;
  stream << "\t" << "(s): (" << b.s << ")" << endl;
  stream << "\t" << "size: " << b.depth << endl;
  for (int i = 0; i < b.depth; i++) {
    stream << "\t" << "<channel, data>: <" << b.channel_idx[i] << ", " << b.data[i] << ">" << endl;
  }
  for (int i = 0; i < b.prk_depth; i++) {
    stream << "\t" << "<p, r, k>: <" << b.pos_ptr[i] << ", " << b.r_idx[i] << ", " << b.k_idx[i] << ">" << endl;
  }
  stream << "======================" << endl;
}

class TestPE {
  public:
    TestPE() {num_of_layer = 0;}

    void SetConvConfig(specs::ConvConfig);
    void InitWeightAndIAAndAns();
    specs::InputActivationBundle GenIABundle(int, int);
    specs::WeightBundle GenWBundle(int);
    void TestConvLayer(specs::ConvConfig, ostream&);
    void TestBundle(int, int, int, ostream&);
    void CalGolden();
    void Validate(ostream& stream = std::cerr);
    
    PE my_pe;
    AssociativeIndexMatchingUnit my_aim;
    
  private:
    int num_of_layer;
    vector<specs::ValidPositionPair> PseudoAIM(specs::InputActivationBundle, specs::WeightBundle);
    void UpdateAns(specs::RawData, ostream&);
    specs::ConvConfig conv_config_;
    vector<vector<vector<vector<int>>>> weight_; // (k, r, s, c)
    vector<vector<vector<int>>> ia_feature_map_; // (h, w, c)
    vector<vector<vector<int>>> my_ans_;
    vector<vector<vector<int>>> golden_;
};

void TestPE::SetConvConfig(specs::ConvConfig conv_config) {
  this->conv_config_ = conv_config;
  this->my_pe.SetConvConfig(conv_config);
  this->InitWeightAndIAAndAns();
  this->CalGolden();
}

specs::InputActivationBundle TestPE::GenIABundle(int h, int w) {
  specs::InputActivationBundle ia_bundle;
  ia_bundle.h = h;
  ia_bundle.w = w;
  int depth = 0;
  for (int c = 0; c < this->conv_config_.ia_channel; c++) {
    if (this->ia_feature_map_[h][w][c]) {
      depth++;
      ia_bundle.channel_idx.push_back(c);
      ia_bundle.data.push_back(this->ia_feature_map_[h][w][c]);
    }
  }
  ia_bundle.depth = depth;
  return ia_bundle;
}

specs::WeightBundle TestPE::GenWBundle(int s) {
  specs::WeightBundle w_bundle;
  w_bundle.s = s;
  int depth = 0;
  int prk_depth = 0;
  for (int k = 0; k < this->conv_config_.oa_channel; k++) {
    for (int r = 0; r < this->conv_config_.kernel_size; r++) {
      for (int c = 0; c < this->conv_config_.ia_channel; c++) {
        if (this->weight_[k][r][s][c]) {
          // if (s == 0) cout << "(k, r, s, c): (" << k << ", " << r << ", " << s << ", " << c << ") -> " <<  this->weight_[k][r][s][c] << endl;
          if (w_bundle.pos_ptr.size() == 0) {
            prk_depth = 1;
            w_bundle.pos_ptr.push_back(0);
            w_bundle.r_idx.push_back(r);
            w_bundle.k_idx.push_back(k);
          }
          else if (w_bundle.r_idx.back() != r || w_bundle.k_idx.back() != k) {
            prk_depth++;
            w_bundle.pos_ptr.push_back(depth);
            w_bundle.r_idx.push_back(r);
            w_bundle.k_idx.push_back(k);
          }
          depth++;
          w_bundle.channel_idx.push_back(c);
          w_bundle.data.push_back(this->weight_[k][r][s][c]);
        }
      }
    }
  }
  w_bundle.depth = depth;
  w_bundle.prk_depth = prk_depth;
  // if (s==0) cout << "end of w bundle" << endl;

  return w_bundle;
}

void TestPE::InitWeightAndIAAndAns() {
  // init weight
  // weight[filter][r][s][channel]
  int w_k = this->conv_config_.oa_channel;
  int w_c = this->conv_config_.ia_channel;
  int kernel = this->conv_config_.kernel_size;
  this->weight_ = vector<vector<vector<vector<int>>>>(
    w_k, vector<vector<vector<int>>>(kernel, vector<vector<int>>(kernel, vector<int>(w_c)))
  );
  int filter = 0;
  int channel = 0;
  int r = 0;
  int s = 0;
  int w = 0;
  for (int i = 0; i < w_k*w_c*kernel*kernel/2; i++) {
    filter = rand() % w_k;
    channel = rand() % w_c;
    r = rand() % kernel;
    s = rand() % kernel;
    w = (rand() % 256)-128;
    this->weight_[filter][r][s][channel] = w;
  }

  // init ia
  int ia_size = this->conv_config_.ia_size;
  int ia_channel = this->conv_config_.ia_channel;
  this->ia_feature_map_ = vector<vector<vector<int>>> (
    ia_size, vector<vector<int>>(ia_size, vector<int>(ia_channel))
  );
  int ia_h = 0;
  int ia_w = 0;
  int ia_c = 0;
  int value = 0;
  for (int i = 0; i < ia_size*ia_size*ia_channel/2; i++) {
    ia_h = rand() % ia_size;
    ia_w = rand() % ia_size;
    ia_c = rand() % ia_channel;
    value = (rand() % 256)-128;
    this->ia_feature_map_[ia_h][ia_w][ia_c] = value;
  }

  // init ans & golden
  int size = this->conv_config_.oa_size;
  int oa_k = this->conv_config_.oa_channel;
  this->my_ans_ = vector<vector<vector<int>>>(size, vector<vector<int>>(size, vector<int>(oa_k)));
  this->golden_ = vector<vector<vector<int>>>(size, vector<vector<int>>(size, vector<int>(oa_k)));
}

void TestPE::TestConvLayer(specs::ConvConfig conv_config, ostream& stream) {
  cout << "Testing Layer " << this->num_of_layer << endl;
  this->SetConvConfig(conv_config);
  for (int i = 0; i < conv_config.ia_size; i++) {
    for (int j = 0; j < conv_config.ia_size; j++) {
      // cout << "(i, j) = (" << i << ", " << j << ")" << endl;
      for (int s = 0; s < conv_config.kernel_size; s++) {
        this->TestBundle(i, j, s, stream);
      }
    }
  }
  this->Validate();
  cout << "Finish Testing Layer " << this->num_of_layer++ << endl;
}

void TestPE::TestBundle(int h, int w, int s, ostream& stream) {
  // FIXME: aim reset
  AssociativeIndexMatchingUnit new_aim;
  this->my_aim = new_aim;

  bool print_val_pos = false;

  stream << "\tProcessing bundle at ia (" << h << ", " << w << "), w at s = " << s << endl; 
  specs::InputActivationBundle ia_bundle = this->GenIABundle(h, w);
  specs::WeightBundle w_bundle = this->GenWBundle(s);
  PrintIABundle(ia_bundle, stream);
  PrintWBundle(w_bundle, stream);
  if (ia_bundle.depth == 0 || w_bundle.depth == 0) return;

  // val pos
  stream << "\tAIM processing..." << endl;
  // this->my_aim.InputBundle(ia_bundle, w_bundle);
  // stream << "\tval " << endl;
  // vector<specs::ValidPositionPair> val_pos_pairs = this->my_aim.GetValidPositionPairs();
  // if (print_val_pos) {
  //   stream << "\tval pos pairs from aim:" << endl;
  //   stream << val_pos_pairs.size() << endl;
  //   for (int i = 0; i < val_pos_pairs.size(); i++) {
  //     stream << "\t<val, pos> = <" << val_pos_pairs[i].valid << ", " << val_pos_pairs[i].pos << ">" << endl;
  //   }
  // }
  
  // FIXME: handle this in AIM
  // vector<specs::ValidPositionPair> new_val_pos_pairs;
  // for (int i = 0; i < val_pos_pairs.size(); i++) {
  //   specs::ValidPositionPair vp;
  //   if (!val_pos_pairs[i].valid) {
  //     int num_of_invalid = 1;
  //     while (i+num_of_invalid < val_pos_pairs.size() && !val_pos_pairs[i+num_of_invalid].valid) {
  //       num_of_invalid++;
  //     }
  //     i += (num_of_invalid-1);
  //     vp.pos = num_of_invalid;
  //     vp.valid = 0;
  //   }
  //   else {
  //     vp = val_pos_pairs[i];
  //   }
  //   if (i >= val_pos_pairs.size()-1 && !vp.valid) break; // if invalid til the end, then simply remove all these pairs
  //   else new_val_pos_pairs.push_back(vp);
  // }
  // if (print_val_pos) {
  //   stream << "\tnew val pos" << endl;
  //   for (int i = 0; i < new_val_pos_pairs.size(); i++) {
  //     stream << "\t<val, pos> = <" << new_val_pos_pairs[i].valid << ", " << new_val_pos_pairs[i].pos << ">" << endl;
  //   }
  // }
  vector<specs::ValidPositionPair> my_pairs = this->PseudoAIM(ia_bundle, w_bundle);
  if (print_val_pos) {
    stream << "my val pos" << endl;
    for (int i = 0; i < my_pairs.size(); i++) {
      stream << "\t<val, pos> = <" << my_pairs[i].valid << ", " << my_pairs[i].pos << ">" << endl;
    }
  }

  // pe
  stream << "\tPE input..." << endl;
  this->my_pe.InputIABundle(ia_bundle);
  this->my_pe.InputWBundle(w_bundle);
  this->my_pe.InputValidPosition(my_pairs);
  stream << "\tPE process..." << endl;
  this->my_pe.Process(stream);
  while (!this->my_pe.finish || this->my_pe.output_valid) {
    if (this->my_pe.output_valid) {
      this->my_pe.PrintFIFO(stream);
      specs::RawData psum = this->my_pe.GetPsum(stream);
      this->UpdateAns(psum, stream);
      stream << "\tReceive partial sum" << endl;
    }
    else if (!this->my_pe.finish) {
      this->my_pe.Process(stream);
    }
  }
  
  
  // TODO validation
}

vector<specs::ValidPositionPair> TestPE::PseudoAIM(specs::InputActivationBundle ia_bundle, specs::WeightBundle w_bundle) {
  vector<specs::ValidPositionPair> val_pos_pairs;
  for (int i = 0; i < w_bundle.depth; i++) {
    specs::ValidPositionPair vp;
    int w_c = w_bundle.channel_idx[i];
    for (int j = 0; j < ia_bundle.depth; j++) {
      if (ia_bundle.channel_idx[j] == w_c) {
        vp.pos = j;
        vp.valid = 1;
        break;
      }
      else if (j == ia_bundle.depth-1) {
        vp.pos = -1;
        vp.valid = 0;
        break;
      }
    }
    val_pos_pairs.push_back(vp);
  }

  vector<specs::ValidPositionPair> new_val_pos_pairs;
  for (int i = 0; i < val_pos_pairs.size(); i++) {
    specs::ValidPositionPair vp;
    if (!val_pos_pairs[i].valid) {
      int num_of_invalid = 1;
      while (i+num_of_invalid < val_pos_pairs.size() && !val_pos_pairs[i+num_of_invalid].valid) {
        num_of_invalid++;
      }
      i += (num_of_invalid-1);
      vp.pos = num_of_invalid;
      vp.valid = 0;
    }
    else {
      vp = val_pos_pairs[i];
    }
    if (i >= val_pos_pairs.size()-1 && !vp.valid) break; // if invalid til the end, then simply remove all these pairs
    else new_val_pos_pairs.push_back(vp);
  }
  return new_val_pos_pairs;
}

void TestPE::UpdateAns(specs::RawData psum, ostream& stream) {
  int size = this->conv_config_.oa_size;
  int channel = this->conv_config_.oa_channel;
  for (int i = 0; i < psum.value.size(); i++) {
    int x = psum.h[i];
    int y = psum.w[i];
    int k = psum.k[i];
    if (x < 0 || x > size-1 || y < 0 || y > size-1 || k < 0 || k > channel-1) {
      stream << "partial sum out of bound!!!" << endl;
    }
    else {
      stream << "partial sum received: <x, y, k, value> = <" << x << ", " << y << ", " << k << ", " << psum.value[i] << ">" << endl;
      this->my_ans_[x][y][k] += psum.value[i];
    }
  }
  // cout << "update value at (0, 0, 0): " << this->my_ans_[0][0][0] << endl;
}

void TestPE::CalGolden() {
  int size = this->conv_config_.oa_size;
  int oa_k = this->conv_config_.oa_channel;
  this->golden_ = vector<vector<vector<int>>>(size, vector<vector<int>>(size, vector<int>(oa_k)));
  int filter = this->conv_config_.oa_channel;
  int channel = this->conv_config_.ia_channel;
  int ia_w = this->conv_config_.ia_size;
  int ia_h = this->conv_config_.ia_size;
  int w_r = this->conv_config_.kernel_size;
  int w_s = this->conv_config_.kernel_size;
  for (int f = 0; f < filter; f++) {
    for (int c = 0; c < channel; c++) {
      for (int w = 0; w < ia_w; w++) {
        for (int h = 0; h < ia_h; h++) {
          for (int r = 0; r < w_r; r++) {
            for (int s = 0; s < w_s; s++) {
              int x = h-r;
              int y = w-s;
              int k = f;
              if (x < 0 || x > size-1 || y < 0 || y > size-1) continue;
              int psum = this->ia_feature_map_[h][w][c]*this->weight_[k][r][s][c];
              this->golden_[x][y][k] += psum;
              // if (x == 0 && y == 0 && k == 4 && psum != 0) {
              //   cout << "(h, w, r, s, k, c): (" << h << ", " << w << ", " << r << ", " << s << ", " << k << ", " << c << ") -> " << psum << endl;
              //   cout << "weight: " << this->weight_[k][r][s][c] << endl;
              //   cout << "ia: " << this->ia_feature_map_[h][w][c] << endl;
              // }
            }
          }
        }
      }
    }
  }
  // for (int i = 0; i < this->conv_config_.ia_channel; i++) {
  //   cout << "ia[0][0][" << i << "]: " << this->ia_feature_map_[0][0][i] << endl;
  //   cout << "w[0][0][0][" << i << "]: " << this->weight_[0][0][0][i] << endl;
  // }
  // cout << "golden at (0, 0, 4): " << this->golden_[0][0][4] << endl;
}

void TestPE::Validate(ostream& stream) {
  int bug_count = 0;
  for (int i = 0; i < this->conv_config_.oa_size; i++) {
    for (int j = 0; j < this->conv_config_.oa_size; j++) {
      for (int k = 0; k < this->conv_config_.oa_channel; k++) {
        if (this->my_ans_[i][j][k] != this->golden_[i][j][k]) { 
          bug_count++;
          cerr << "error occur at (i, j, k) = (" << i << ", " << j << ", " << k << ")" << endl;
        }
        // else {
        //   cerr << "correct at (i, j, k) = (" << i << ", " << j << ", " << k << ")" << endl;
        // }
      }
    }
  }
  if (bug_count) cerr << "There are " << bug_count << " bugs!" << endl;
  else cerr << "All correct!" << endl;
}

specs::ConvConfig CONV_1_CONFIG;
specs::ConvConfig CONV_2_CONFIG;
specs::ConvConfig CONV_3_CONFIG;
specs::ConvConfig CONV_4_CONFIG;
specs::ConvConfig CONV_5_CONFIG;
specs::ConvConfig ALEX_1_CONFIG;

void InitConvConfig() {
  // normal 3x3 kernel
  CONV_1_CONFIG.bias = -1;
  CONV_1_CONFIG.relu = true;
  CONV_1_CONFIG.stride = 1;
  CONV_1_CONFIG.ia_size = 7;
  CONV_1_CONFIG.ia_channel = 3;
  CONV_1_CONFIG.oa_size = 5;
  CONV_1_CONFIG.oa_channel = 5;
  CONV_1_CONFIG.kernel_size = 3;
  CONV_1_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_1_CONFIG.num_of_oa_bundle = 5*5;


  // normal 5x5 kernel
  CONV_2_CONFIG.bias = 100;
  CONV_2_CONFIG.relu = true;
  CONV_2_CONFIG.stride = 1;
  CONV_2_CONFIG.oa_size = 8;
  CONV_2_CONFIG.oa_channel = 5;
  CONV_2_CONFIG.kernel_size = 3;
  CONV_2_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_2_CONFIG.num_of_oa_bundle = 8*8;

  // normal 4x4 kernel
  CONV_3_CONFIG.bias = 20;
  CONV_3_CONFIG.relu = true;
  CONV_3_CONFIG.stride = 1;
  CONV_3_CONFIG.ia_size = 11;
  CONV_3_CONFIG.ia_channel = 8;
  CONV_3_CONFIG.oa_size = 8;
  CONV_3_CONFIG.oa_channel = 4;
  CONV_3_CONFIG.kernel_size = 4;
  CONV_3_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_3_CONFIG.num_of_oa_bundle = 8*8;

  // big feature map
  CONV_4_CONFIG.bias = -15;
  CONV_4_CONFIG.relu = false;
  CONV_4_CONFIG.stride = 1;
  CONV_4_CONFIG.ia_size = 127;
  CONV_4_CONFIG.ia_channel = 3;
  CONV_4_CONFIG.oa_size = 125;
  CONV_4_CONFIG.oa_channel = 33;
  CONV_4_CONFIG.kernel_size = 3;
  CONV_4_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_4_CONFIG.num_of_oa_bundle = 8*8;

  // big feature map
  CONV_5_CONFIG.bias = 102;
  CONV_5_CONFIG.relu = false;
  CONV_5_CONFIG.stride = 1;
  CONV_5_CONFIG.ia_size = 9;
  CONV_5_CONFIG.ia_channel = 96;
  CONV_5_CONFIG.oa_size = 7;
  CONV_5_CONFIG.oa_channel = 96;
  CONV_5_CONFIG.kernel_size = 3;
  CONV_5_CONFIG.num_of_w_bundle = 0; // FIXME
  CONV_5_CONFIG.num_of_oa_bundle = 31*31;

}

int main() {
  std::string log_file_path = "logfile.log";
  ofstream myfile;
  myfile.open(log_file_path);
  ostream& stream = myfile;

  InitConvConfig();
  TestPE tb;
  cout << "Start Test" << endl;
  tb.TestConvLayer(CONV_5_CONFIG, stream);
  tb.TestConvLayer(CONV_4_CONFIG, stream);
  tb.TestConvLayer(CONV_1_CONFIG, stream);
  cout << "Finish All Test!!!" << endl;
  // tb.SetConvConfig(CONV_1_CONFIG);
  // tb.TestBundle(0, 0, 0, stream);



  myfile.close();
}