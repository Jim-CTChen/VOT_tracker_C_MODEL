#include "../inc/controller.h"
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "../inc/specs.h"
#include "../inc/state.h"
#include "../inc/AIM/associative_index_matching_unit.h"
#include "../inc/PE/pe.h"
#include "../inc/tool.h"


const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}

std::vector<specs::ValidPositionPair> Controller::PseudoAIM(specs::InputActivationBundle ia_bundle, specs::WeightBundle w_bundle) {
  std::vector<specs::ValidPositionPair> val_pos_pairs;
  if (!ia_bundle.depth) return val_pos_pairs;
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

  std::vector<specs::ValidPositionPair> new_val_pos_pairs;
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

Controller::Controller(int num_of_PE_set, int num_of_PE_in_set) {
  // TODO
  // init each module

  // construct aim array & PEs
  this->num_of_PE_set_ = num_of_PE_set;
  this->num_of_PE_in_set_ = num_of_PE_in_set;
  for (int i = 0; i < num_of_PE_set; i++) {
    aims_.push_back(AssociativeIndexMatchingUnit());
    std::vector<PE> pe_set;
    for (int j = 0; j < num_of_PE_in_set; j++) {
      pe_set.push_back(PE());
    }
    this->PEs_.push_back(pe_set);
  }

  // init convolution config
  this->reducer_ = Reducer(num_of_PE_set);
  InitConvConfig();
}

void Controller::Init() {
  bool log = true;
  bool resume = false;
  std::cout << "Initializing DRAM" << std::endl;
  if (resume) {
    this->InitDramOA("./test/imgz_conv6_output.txt");
  }
  else {
    this->InitDramOA("./test/imgz_conv1_input.txt");
    this->ImgZAlexnet(log);
  }
  std::cout << "C_Z_K..." << std::endl;
  this->Convolution(C_Z_K, WEIGHT);
  if (log) this->ReadZKernel(C_Z.w_bundle_set_index, C_Z.kernel_size, "C_Z_K_oa");

  std::cout << "R_Z_K..." << std::endl;
  this->Convolution(R_Z_K, WEIGHT);
  if (log) this->ReadZKernel(R_Z.w_bundle_set_index, R_Z.kernel_size, "R_Z_K_oa");


  // std::string dram_log_file_path = "dram.log";
  // std::ofstream dramfile;
  // dramfile.open(dram_log_file_path);
  // std::ostream& dram_stream = dramfile;
  // PrintDram(mem_controller_, dram_stream);
  // dramfile.close();
  // std::cout << "Finish Init!!!" << std::endl;
}

void Controller::Track() {
  bool log = true;
  bool resume = false;
  std::cout << "Initializing DRAM OA" << std::endl;
  if (resume) {
    this->InitDramOA("./test/imgx_conv6_output.txt");
    // this->InitDramOA("./test/c_x.txt");
    // this->InitDramOA("./test/c_z_k.txt");
  }
  else {
    this->InitDramOA("./test/imgx_conv1_input.txt");
    this->ImgXAlexnet(log);
  }
  std::cout << "C_X..." << std::endl;
  this->Convolution(C_X);
  if (log) this->ReadOA(C_X.oa_bundle_set_index, C_X.num_of_oa_bundle, "C_X_oa");


  std::cout << "R_X..." << std::endl;
  this->Convolution(R_X);
  if (log) this->ReadOA(R_X.oa_bundle_set_index, R_X.num_of_oa_bundle, "R_X_oa");

  

  std::cout << "C_Z..." << std::endl;
  this->XCorr(C_Z, true);
  if (log) this->ReadOA(C_Z.oa_bundle_set_index, C_Z.num_of_oa_bundle, "C_Z_oa");

  // this->InitDramOA("./test/cls_conv1_input.txt");
  std::cout << "C_Conv1..." << std::endl;
  this->Convolution(C_CONV1);
  if (log) this->ReadOA(C_CONV1.oa_bundle_set_index, C_CONV1.num_of_oa_bundle, "C_Conv1_oa");

  // this->InitDramOA("./test/cls_conv2_input.txt");
  std::cout << "C_Conv2..." << std::endl;
  this->Convolution(C_CONV2);
  if (log) this->ReadOA(C_CONV2.oa_bundle_set_index, C_CONV2.num_of_oa_bundle, "C_Conv2_oa");

  // this->InitDramOA("./test/cls_conv3_input.txt");
  std::cout << "C_Conv3..." << std::endl;
  this->Convolution(C_CONV3);
  if (log) this->ReadOA(C_CONV3.oa_bundle_set_index, C_CONV3.num_of_oa_bundle, "C_Conv3_oa");
  
  // this->InitDramOA("./test/cls_score_input.txt");
  std::cout << "C_cls_conv..." << std::endl;
  this->Convolution(C_CLS_CONV, SCORE, true);

  std::cout << "C_ctr_conv..." << std::endl;
  this->Convolution(C_CTR_CONV, SCORE);



  std::cout << "R_Z..." << std::endl;
  this->XCorr(R_Z);
  if (log) this->ReadOA(R_Z.oa_bundle_set_index, R_Z.num_of_oa_bundle, "R_Z_oa");

  std::cout << "R_Conv1..." << std::endl;
  this->Convolution(R_CONV1);
  if (log) this->ReadOA(R_CONV1.oa_bundle_set_index, R_CONV1.num_of_oa_bundle, "R_Conv1_oa");

  std::cout << "R_Conv2..." << std::endl;
  this->Convolution(R_CONV2);
  if (log) this->ReadOA(R_CONV2.oa_bundle_set_index, R_CONV2.num_of_oa_bundle, "R_Conv2_oa");

  std::cout << "R_Conv3..." << std::endl;
  this->Convolution(R_CONV3);
  if (log) this->ReadOA(R_CONV3.oa_bundle_set_index, R_CONV3.num_of_oa_bundle, "R_Conv3_oa");

  // this->InitDramOA("./test/offset_input.txt");
  std::cout << "R_Reg..." << std::endl;
  this->Convolution(R_REG_CONV, SCORE, true);


  std::string dram_log_file_path = "dram_result.txt";
  std::ofstream dramfile;
  dramfile.open(dram_log_file_path);
  std::ostream& dram_stream = dramfile;
  PrintDram(mem_controller_, dram_stream);
  dramfile.close();
}

void Controller::Clear(){
  return;
}

void Controller::InitDramWeight(std::string filename) {
  // weight
  std::vector<std::string> temp_s;
  std::ifstream weight_file;
  std::string s;
  std::vector<int> temp(kDramAccessBytes,0);
  weight_file.open(filename);
  int address;

  int stop=0;
  while(getline(weight_file,s)) // keep reading until we run out
  {
    // if (stop>=10000){
    //   break;
    // }
    stop++;

    temp_s = split(s, "  ");
    address = stoi(temp_s[0]);
    temp_s.erase(temp_s.begin()); //address
    temp_s.erase(temp_s.begin()); //[
    temp_s.erase(temp_s.begin()+temp_s.size()-1); //]

    for(int i=0; i<temp_s.size(); i++){
      temp[i] = stoi(temp_s[i]);
    }

    // cout << "address: "<< address<<endl;

    this->mem_controller_.dram_.mem_[address] = temp;
    std::fill(temp.begin(), temp.end(), 0);
  }

  weight_file.close();
}

void Controller::InitDramOA(std::string filename) {
  // OA
  std::vector<std::string> temp_s;
  std::ifstream ia_file;
  std::string s;
  std::vector<int> temp(kDramAccessBytes,0);
  ia_file.open(filename);
  int address;

  int stop=0;
  while(getline(ia_file,s)) // keep reading until we run out
  {
    // if (stop>=10000){
    //   break;
    // }
    stop++;

    temp_s = split(s, "  ");
    address = stoi(temp_s[0]);
    temp_s.erase(temp_s.begin()); //address
    temp_s.erase(temp_s.begin()); //[
    temp_s.erase(temp_s.begin()+temp_s.size()-1); //]

    for(int i=0; i<temp_s.size(); i++){
      temp[i] = stoi(temp_s[i]);
    }
    // cout << "address: "<< address<<endl;

    this->mem_controller_.dram_.mem_[address] = temp;
    std::fill(temp.begin(), temp.end(), 0);
  }

  ia_file.close();
}

void Controller::Test() {
  std::string ia_filename = "./test/conv1_input.txt";
  // specs::ConvConfig conv_config = TEST_CONV_S2;
  specs::ConvConfig conv_config = ALEXNET_IMGZ_CONV1;
  // specs::MaxPoolConfig max_config = ALEXNET_IMGZ_MAXPOOL2;

  std::cout << "Initializing DRAM" << std::endl;
  this->InitDramOA(ia_filename);
  std::cout << "Finish Initialization" << std::endl;
  // this->TestMaxPool(max_config);
  // this->TestConv(conv_config);
  // conv_config = ALEXNET_IMGZ_CONV6;
  this->TestConv(conv_config);
  std::cout << "Validating" << std::endl;
  this->ReadOA(conv_config.oa_bundle_set_index, conv_config.num_of_oa_bundle, "oa");
  // this->ReadTestOA(max_config.oa_bundle_set_index, max_config.num_of_oa_bundle);
  std::cout << "Finish Test" << std::endl;
}

void Controller::ImgZAlexnet(bool log) {
  std::cout << "Start Alexnet Imgz" << std::endl;
  std::cout << "Conv1..." << std::endl;
  this->Convolution(ALEXNET_IMGZ_CONV1);
  if (log) this->ReadOA(ALEXNET_IMGZ_CONV1.oa_bundle_set_index, ALEXNET_IMGZ_CONV1.num_of_oa_bundle, "alexnet_imgz_conv1_oa");

  std::cout << "Conv2..." << std::endl;
  this->Convolution(ALEXNET_IMGZ_CONV2);
  if (log) this->ReadOA(ALEXNET_IMGZ_CONV2.oa_bundle_set_index, ALEXNET_IMGZ_CONV2.num_of_oa_bundle, "alexnet_imgz_conv2_oa");

  std::cout << "Max Pool1..." << std::endl;
  this->MaxPool(ALEXNET_IMGZ_MAXPOOL1);
  if (log) this->ReadOA(ALEXNET_IMGZ_MAXPOOL1.oa_bundle_set_index, ALEXNET_IMGZ_MAXPOOL1.num_of_oa_bundle, "alexnet_imgz_maxpool1_oa");

  std::cout << "Conv3..." << std::endl;
  this->Convolution(ALEXNET_IMGZ_CONV3);
  if (log) this->ReadOA(ALEXNET_IMGZ_CONV3.oa_bundle_set_index, ALEXNET_IMGZ_CONV3.num_of_oa_bundle, "alexnet_imgz_conv3_oa");
  std::cout << "Max Pool2..." << std::endl;
  this->MaxPool(ALEXNET_IMGZ_MAXPOOL2);
  if (log) this->ReadOA(ALEXNET_IMGZ_MAXPOOL2.oa_bundle_set_index, ALEXNET_IMGZ_MAXPOOL2.num_of_oa_bundle, "alexnet_imgz_maxpool2_oa");
  std::cout << "Conv4..." << std::endl;
  this->Convolution(ALEXNET_IMGZ_CONV4);
  if (log) this->ReadOA(ALEXNET_IMGZ_CONV4.oa_bundle_set_index, ALEXNET_IMGZ_CONV4.num_of_oa_bundle, "alexnet_imgz_conv4_oa");
  std::cout << "Conv5..." << std::endl;
  this->Convolution(ALEXNET_IMGZ_CONV5);
  if (log) this->ReadOA(ALEXNET_IMGZ_CONV5.oa_bundle_set_index, ALEXNET_IMGZ_CONV5.num_of_oa_bundle, "alexnet_imgz_conv5_oa");
  std::cout << "Conv6..." << std::endl;
  this->Convolution(ALEXNET_IMGZ_CONV6);
  if (log) this->ReadOA(ALEXNET_IMGZ_CONV6.oa_bundle_set_index, ALEXNET_IMGZ_CONV6.num_of_oa_bundle, "alexnet_imgz_conv6_oa");

  std::cout << "Finish Alexnet Imgz!!!" << std::endl;

}

void Controller::ImgXAlexnet(bool log) {
  std::cout << "Start testing Alexnet Imgx" << std::endl;
  std::cout << "Conv1..." << std::endl;
  this->Convolution(ALEXNET_IMGX_CONV1);
  this->ReadOA(ALEXNET_IMGX_CONV1.oa_bundle_set_index, ALEXNET_IMGX_CONV1.num_of_oa_bundle, "alexnet_imgx_conv1_oa");

  std::cout << "Conv2..." << std::endl;
  this->Convolution(ALEXNET_IMGX_CONV2);
  this->ReadOA(ALEXNET_IMGX_CONV2.oa_bundle_set_index, ALEXNET_IMGX_CONV2.num_of_oa_bundle, "alexnet_imgx_conv2_oa");

  std::cout << "Max Pool1..." << std::endl;
  this->MaxPool(ALEXNET_IMGX_MAXPOOL1);
  this->ReadOA(ALEXNET_IMGX_MAXPOOL1.oa_bundle_set_index, ALEXNET_IMGX_MAXPOOL1.num_of_oa_bundle, "alexnet_imgx_maxpool1_oa");

  std::cout << "Conv3..." << std::endl;
  this->Convolution(ALEXNET_IMGX_CONV3);
  this->ReadOA(ALEXNET_IMGX_CONV3.oa_bundle_set_index, ALEXNET_IMGX_CONV3.num_of_oa_bundle, "alexnet_imgx_conv3_oa");
  std::cout << "Max Pool2..." << std::endl;
  this->MaxPool(ALEXNET_IMGX_MAXPOOL2);
  this->ReadOA(ALEXNET_IMGX_MAXPOOL2.oa_bundle_set_index, ALEXNET_IMGX_MAXPOOL2.num_of_oa_bundle, "alexnet_imgx_maxpool2_oa");
  std::cout << "Conv4..." << std::endl;
  this->Convolution(ALEXNET_IMGX_CONV4);
  this->ReadOA(ALEXNET_IMGX_CONV4.oa_bundle_set_index, ALEXNET_IMGX_CONV4.num_of_oa_bundle, "alexnet_imgx_conv4_oa");
  std::cout << "Conv5..." << std::endl;
  this->Convolution(ALEXNET_IMGX_CONV5);
  this->ReadOA(ALEXNET_IMGX_CONV5.oa_bundle_set_index, ALEXNET_IMGX_CONV5.num_of_oa_bundle, "alexnet_imgx_conv5_oa");
  std::cout << "Conv6..." << std::endl;
  this->Convolution(ALEXNET_IMGX_CONV6);
  this->ReadOA(ALEXNET_IMGX_CONV6.oa_bundle_set_index, ALEXNET_IMGX_CONV6.num_of_oa_bundle, "alexnet_imgx_conv6_oa");

  std::cout << "Finish Alexnet Imgx!!!" << std::endl;

}

void Controller::TestXcorr() {
  std::string ia_filename = "./test/imgz_conv6_output.txt";
  std::cout << "Initializing DRAM" << std::endl;
  this->InitDramOA(ia_filename);

  std::cout << "C_Z_K..." << std::endl;
  ia_filename = "./test/c_z_k.txt";
  this->InitDramOA(ia_filename);
  // this->ZK_Convolution(C_Z_K);
  std::cout << "logging OA..." << std::endl;
  this->ReadZKernel(C_Z.w_bundle_set_index, C_Z.kernel_size, "C_Z_K_oa");


  // std::cout << "R_Z_K..." << std::endl;
  // this->ZK_Convolution(R_Z_K);

  std::cout << "C_X..." << std::endl;
  // ia_filename =  "./test/imgx_conv6_output.txt";
  ia_filename = "./test/c_x.txt";
  this->InitDramOA(ia_filename);
  // this->Convolution(C_X);
  // this->ReadOA(C_X.oa_bundle_set_index, C_X.num_of_oa_bundle, "C_X_oa");
  // std::cout << "R_X..." << std::endl;
  // this->Convolution(R_X);


  std::cout << "C_Z..." << std::endl;
  this->XCorr(C_Z);

  std::cout << "logging OA..." << std::endl;
  this->ReadOA(C_Z.oa_bundle_set_index, C_Z.num_of_oa_bundle, "C_Z_oa");


  // std::string dram_log_file_path = "dram.log";
  // std::ofstream dramfile;
  // dramfile.open(dram_log_file_path);
  // std::ostream& dram_stream = dramfile;
  // PrintDram(mem_controller_, dram_stream);
  // dramfile.close();

  std::cout << "Finish" << std::endl;
}

void Controller::PrintPsum(std::vector<int> bias, specs::ConvConfig conv_config, std::ostream& stream) {
  stream << "Printing psum (h, w, k, value):" << std::endl;
  for (int i = 0; i < this->psum_.size(); i++) {
    for (int j = 0; j < this->psum_[i].size(); j++) {
      for (int k = 0; k < this->psum_[i][j].size(); k++) {
        stream << "(" << i << ", " << j << ", " << k << ", " << (((this->psum_[i][j][k]+bias[k])*conv_config.quantize_M) >> conv_config.quantize_n) << ")" << std::endl;
      }
    }
  }
}

void Controller::UpdatePsum(specs::RawData psum) {
  assert (psum.h.size() == psum.w.size());
  assert (psum.h.size() == psum.k.size());
  assert (psum.h.size() == psum.value.size());
  for (int i = 0; i < psum.h.size(); i++) {
    int h = psum.h[i];
    int w = psum.w[i];
    int k = psum.k[i];
    int value = psum.value[i];
    this->psum_[h][w][k] += value;
  }
}

void Controller::TestMaxPool(specs::MaxPoolConfig max_config) {
  std::ofstream ia_file;
  ia_file.open("ia.log");
  std::ostream& stream = ia_file;

  int ia_bundle_num = max_config.num_of_ia_bundle;
  int oa_bundle_num = max_config.num_of_oa_bundle;

  this->mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(max_config.ia_bundle_set_index);
  this->mem_controller_.InputCalibrate(max_config.ia_bundle_set_index);
  this->mem_controller_.OutputCalibrate(max_config.oa_bundle_set_index);

  this->max_pool_.SetConfig(max_config);
  int current_bundle_index = 0;
  int oa_bundle_index = 0;
  while (current_bundle_index != max_config.num_of_ia_bundle) {
    std::cout << "current ia bundle index: " << current_bundle_index << std::endl;
    this->max_pool_.SetReducerIABundleIndex(current_bundle_index);
    bool output = this->max_pool_.request;
    while (this->max_pool_.request) {
      int requested_bundle_index = this->max_pool_.request_bundle_index;
      specs::InputActivationBundle ia_bundle = this->mem_controller_.ReadInputActivationBundleFromIaSram(requested_bundle_index);
      this->max_pool_.SendBundle(ia_bundle);
    }
    if (output) {
      specs::InputActivationBundle oa_bundle = this->max_pool_.GetOABundle();
      this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_bundle_index);
      oa_bundle_index++;
    }
    current_bundle_index++;
  }
}

void Controller::TestConv(specs::ConvConfig conv_config) {
  bool debug = false;

  std::string log_file_path = "logfile.log";
  std::ofstream myfile;
  myfile.open(log_file_path);
  std::ostream& stream = myfile;

  std::ofstream ia_file;
  ia_file.open("ia.log");
  std::ostream& ia_stream = ia_file;

  std::ofstream w_file;
  w_file.open("w.log");
  std::ostream& w_stream = w_file;

  std::ofstream reducer_output_file;
  reducer_output_file.open("reducer.log");
  std::ostream& reducer_stream = reducer_output_file;

  this->psum_ = std::vector<std::vector<std::vector<int>>>(conv_config.oa_size, std::vector<std::vector<int>>(conv_config.oa_size, std::vector<int>(conv_config.oa_channel)));

  int ia_bundle_num = conv_config.num_of_ia_bundle;
  int w_bundle_num  = conv_config.num_of_w_bundle;

  // assume SRAM is big enough to save a whole bundle set
  // move weight bundle set from DRAM to SRAM
  std::cout << "Moving weight & ia from DRAM" << std::endl;
  mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(conv_config.ia_bundle_set_index);
  mem_controller_.MoveWeightBundleSetFromDramToWSram(conv_config.w_bundle_set_index);

  // config reducer, mem controller, PE
  std::cout << "Setting convolution config for reducer, memory controller, PEs" << std::endl;
  this->compressor_.SetOAChannel(conv_config.oa_channel);
  mem_controller_.InputCalibrate(conv_config.ia_bundle_set_index); // im_z_crop 127x127x3
  mem_controller_.OutputCalibrate(conv_config.oa_bundle_set_index);
  mem_controller_.WeightCalibrate(conv_config.w_bundle_set_index);
  for (int i = 0; i < this->num_of_PE_set_; i++) {
    for (int j = 0; j < this->num_of_PE_in_set_; j++) {
      this->PEs_[i][j].SetConvConfig(conv_config);
    }
  }
  // read bias for this layer
  std::cout << "Read bias" << std::endl;  
  std::vector<int> bias = mem_controller_.ReadWeightBiasFromWSram(conv_config.w_bundle_set_index);
  // PrintBias(bias);
  this->reducer_.SetConvConfig(conv_config, bias);
  // this->reducer_.PrintBuffer(true, stream);


  // TODO
  // consider buffer(RF) for bundle
  // since SRAM will take too much time
  bool print = true;
  std::cout << "Start processing..." << std::endl;
  int ia_idx = 0; // ia bundle index
  int w_idx = 0;  // w bundle index
  int oa_idx = 0;
  int smallest_ia_h = 0;
  int smallest_ia_w = 0;
  std::vector<specs::InputActivationBundle> ia_bundle_batch;
  std::vector<specs::WeightBundle> w_bundle_batch;

  // std::string dram_log_file_path = "dram.log";
  // std::ofstream dramfile;
  // dramfile.open(dram_log_file_path);
  // std::ostream& dram_stream = dramfile;
  // PrintDram(mem_controller_, dram_stream);

  
  while (ia_idx < ia_bundle_num) {
    w_idx = 0;
    if (!(ia_idx%100)) std::cout << "Working on ia bundle " << ia_idx << "/" << ia_bundle_num << std::endl;
    ia_bundle_batch.clear();
    smallest_ia_h = ia_idx/conv_config.ia_size;
    smallest_ia_w = ia_idx%conv_config.ia_size;
    // read ia bundles out, input stationary
    for (int i = 0; i < this->num_of_PE_set_; i++) {
      if (ia_idx >= ia_bundle_num) break; // end of MAC operation
      specs::InputActivationBundle b = mem_controller_.ReadInputActivationBundleFromIaSram(ia_idx);
      PrintIABundle(b, ia_stream);
      ia_bundle_batch.push_back(b);
      ia_idx++;
    }
    // std::cout << "1" << std::endl;
    // iterate over all weight bundles
    while (w_idx < w_bundle_num) {
      if (debug) std::cout << "Working on weight bundle " << w_idx << "/" << w_bundle_num << std::endl;
      w_bundle_batch.clear();
      // read w bundles out
      for (int j = 0; j < this->num_of_PE_in_set_; j++) {
        if (w_idx >= w_bundle_num) break;
        specs::WeightBundle b = mem_controller_.ReadWeightBundleFromWSram(w_idx);
        w_bundle_batch.push_back(b);
        PrintWBundle(b, w_file);
        w_idx++;
      }
      if (debug) std::cout << "2" << std::endl;
      for (int l = 0; l < w_bundle_batch.size(); l++) { // not iterate over num_of_PE_in_set for edge condition
        for (int k = 0; k < ia_bundle_batch.size(); k++) {
          // this for-loop execute in same cycle, over each aims and PEs
          // FIXME: replace pseudo AIM with real one
          // this->aims_[k].InputBundle(ia_bundle_batch[k], w_bundle_batch[l]);
          // std::vector<specs::ValidPositionPair> v_p = aims_[k].GetValidPositionPairs();
          if (debug) std::cout << "21" << std::endl;
          std::vector<specs::ValidPositionPair> v_p = this->PseudoAIM(ia_bundle_batch[k], w_bundle_batch[l]);
          if (!v_p.size()) continue;
          if (debug) std::cout << "22" << std::endl;
          this->PEs_[k][l].InputIABundle(ia_bundle_batch[k]);
          this->PEs_[k][l].InputWBundle(w_bundle_batch[l]);
          this->PEs_[k][l].InputValidPosition(v_p);
          if (debug) std::cout << "<ia_idx, w_idx>: <" << k << ", " << l << ">" << std::endl;
          // PrintIABundle(ia_bundle_batch[k]);
          // PrintWBundle(w_bundle_batch[l]);
          // PrintValidPositionPairs(v_p);
          if (debug) std::cout << "23" << std::endl;
          this->PEs_[k][l].Process();
          if (debug) std::cout << "24" << std::endl;
          while (!this->PEs_[k][l].finish || this->PEs_[k][l].output_valid) {
            if (this->PEs_[k][l].output_valid) {
              if (debug) std::cout << "get partial sum" << std::endl;
              specs::RawData psum = this->PEs_[k][l].GetPsum();
              // PrintPartialSum(psum);
              // std::cout << "here" << std::endl;
              // this->UpdatePsum(psum);
              
              // std::cout << "ia anchor (h, w): (" << smallest_ia_h << ", " << smallest_ia_w << ")" << std::endl;
              // std::cout << "psum address (h, w, k) = (" << psum.h[0] << ", " << psum.w[0] << ", " << psum.k[0] << ")" << std::endl;
              this->reducer_.Reduce(psum, smallest_ia_h, smallest_ia_w);
            }
            else if (!this->PEs_[k][l].finish) {
              this->PEs_[k][l].Process();
            }
            // check for output from reducer
            while (this->reducer_.output_ready) {
              // std::cout << "reducer output ready!" << std::endl;
              int h = 0;
              int w = 0;
              std::vector<int> oa = this->reducer_.GetOA(h, w);
              this->compressor_.InputRaw(h, w, oa);
              specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
              // std::cout << "get OA from reducer" << std::endl;
              this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
              PrintIABundle(oa_bundle, reducer_stream);
              if (debug) std::cout << "OA written to DRAM" << std::endl;
              oa_idx++;
            }
          }
          // std::cout << "25" << std::endl;
        }
      }
      // finish one batch (num_of_set x num_of_pe_in_set)
      // check for output from reducer
      while (this->reducer_.output_ready) {
        // std::cout << "8" << std::endl;
        int h = 0;
        int w = 0;
        std::vector<int> oa = this->reducer_.GetOA(h, w);
        this->compressor_.InputRaw(h, w, oa);
        specs::InputActivationBundle oa_bundle = this->compressor_.Compress();

        this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
        PrintIABundle(oa_bundle, reducer_stream);
        oa_idx++;
      }
      // std::cout << "7" << std::endl;
    }
  }
  this->reducer_.AllPartialSumFinish();
  while(!this->reducer_.finish) {
    // std::cout << "oa index: " << oa_idx << std::endl;
    int h = 0;
    int w = 0;
    std::vector<int> oa = this->reducer_.GetOA(h, w);
    this->compressor_.InputRaw(h, w, oa);
    specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
    // std::cout << "writing bundle" << std::endl;
    this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
    PrintIABundle(oa_bundle, reducer_stream);
    if (debug) std::cout << "finish writing" << std::endl;
    oa_idx++;
  }
  // this->PrintPsum(bias, conv_config);
}

void Controller::MaxPool(specs::MaxPoolConfig max_config) {
  int ia_bundle_num = max_config.num_of_ia_bundle;
  int oa_bundle_num = max_config.num_of_oa_bundle;

  this->mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(max_config.ia_bundle_set_index);
  this->mem_controller_.InputCalibrate(max_config.ia_bundle_set_index);
  this->mem_controller_.OutputCalibrate(max_config.oa_bundle_set_index);

  this->max_pool_.SetConfig(max_config);
  int current_bundle_index = 0;
  int oa_bundle_index = 0;
  while (current_bundle_index != max_config.num_of_ia_bundle) {
    this->max_pool_.SetReducerIABundleIndex(current_bundle_index);
    bool output = this->max_pool_.request;
    while (this->max_pool_.request) {
      int requested_bundle_index = this->max_pool_.request_bundle_index;
      specs::InputActivationBundle ia_bundle = this->mem_controller_.ReadInputActivationBundleFromIaSram(requested_bundle_index);
      this->max_pool_.SendBundle(ia_bundle);
    }
    if (output) {
      specs::InputActivationBundle oa_bundle = this->max_pool_.GetOABundle();
      this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_bundle_index);
      oa_bundle_index++;
    }
    current_bundle_index++;
  }
}

void Controller::Convolution(specs::ConvConfig conv_config, OutputType type, bool verbose) {
  std::ofstream reducer_output_file;
  reducer_output_file.open("reducer_offset.log");
  std::ostream& reducer_stream = reducer_output_file;

  std::ofstream ia_file;
  ia_file.open("ia_offset.log");
  std::ostream& ia_stream = ia_file;

  int ia_bundle_num = conv_config.num_of_ia_bundle;
  int w_bundle_num  = conv_config.num_of_w_bundle;

  // assume SRAM is big enough to save a whole bundle set
  // move weight bundle set from DRAM to SRAM
  if (verbose) std::cout << "Moving weight & ia from DRAM" << std::endl;
  mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(conv_config.ia_bundle_set_index);
  mem_controller_.MoveWeightBundleSetFromDramToWSram(conv_config.w_bundle_set_index);

  // config reducer, mem controller, PE
  if (verbose) std::cout << "Setting convolution config for reducer, memory controller, PEs" << std::endl;
  this->compressor_.SetOAChannel(conv_config.oa_channel);
  mem_controller_.InputCalibrate(conv_config.ia_bundle_set_index); // im_z_crop 127x127x3
  mem_controller_.OutputCalibrate(conv_config.oa_bundle_set_index);
  mem_controller_.WeightCalibrate(conv_config.w_bundle_set_index);
  for (int i = 0; i < this->num_of_PE_set_; i++) {
    for (int j = 0; j < this->num_of_PE_in_set_; j++) {
      this->PEs_[i][j].SetConvConfig(conv_config);
    }
  }
  // read bias for this layer
  if (verbose) std::cout << "Read bias" << std::endl;  
  std::vector<int> bias = mem_controller_.ReadWeightBiasFromWSram(conv_config.w_bundle_set_index);
  this->reducer_.SetConvConfig(conv_config, bias);

  // consider buffer(RF) for bundle
  // since SRAM will take too much time
  if (verbose) std::cout << "Start processing..." << std::endl;
  int ia_idx = 0; // ia bundle index
  int w_idx = 0;  // w bundle index
  int oa_idx = 0;
  int smallest_ia_h = 0;
  int smallest_ia_w = 0;
  std::vector<specs::InputActivationBundle> ia_bundle_batch;
  std::vector<specs::WeightBundle> w_bundle_batch;


  while (ia_idx < ia_bundle_num) {
    w_idx = 0;
    if (!(ia_idx%50)) std::cout << "Working on ia bundle " << ia_idx << "/" << ia_bundle_num << std::endl;
    ia_bundle_batch.clear();
    smallest_ia_h = ia_idx/conv_config.ia_size;
    smallest_ia_w = ia_idx%conv_config.ia_size;
    // read ia bundles out, input stationary
    for (int i = 0; i < this->num_of_PE_set_; i++) {
      if (ia_idx >= ia_bundle_num) break; // end of MAC operation
      specs::InputActivationBundle b = mem_controller_.ReadInputActivationBundleFromIaSram(ia_idx);
      ia_bundle_batch.push_back(b);
      PrintIABundle(b, ia_stream);
      ia_idx++;
    }

    // iterate over all weight bundles
    while (w_idx < w_bundle_num) {
      w_bundle_batch.clear();
      // read w bundles out
      for (int j = 0; j < this->num_of_PE_in_set_; j++) {
        if (w_idx >= w_bundle_num) break;
        specs::WeightBundle b = mem_controller_.ReadWeightBundleFromWSram(w_idx);
        w_bundle_batch.push_back(b);
        w_idx++;
      }
      for (int l = 0; l < w_bundle_batch.size(); l++) { // not iterate over num_of_PE_in_set for edge condition
        for (int k = 0; k < ia_bundle_batch.size(); k++) {
          // this for-loop execute in same cycle, over each aims and PEs
          // FIXME: replace pseudo AIM with real one
          // this->aims_[k].InputBundle(ia_bundle_batch[k], w_bundle_batch[l]);
          // std::vector<specs::ValidPositionPair> v_p = aims_[k].GetValidPositionPairs();
          std::vector<specs::ValidPositionPair> v_p = this->PseudoAIM(ia_bundle_batch[k], w_bundle_batch[l]);
          this->PEs_[k][l].InputIABundle(ia_bundle_batch[k]);
          this->PEs_[k][l].InputWBundle(w_bundle_batch[l]);
          this->PEs_[k][l].InputValidPosition(v_p);
          this->PEs_[k][l].Process();
          while (!this->PEs_[k][l].finish || this->PEs_[k][l].output_valid) {
            if (this->PEs_[k][l].output_valid) {
              specs::RawData psum = this->PEs_[k][l].GetPsum();
              this->reducer_.Reduce(psum, smallest_ia_h, smallest_ia_w);
            }
            else if (!this->PEs_[k][l].finish) {
              this->PEs_[k][l].Process();
            }
            // check for output from reducer
            while (this->reducer_.output_ready) {
              int h = 0;
              int w = 0;
              std::vector<int> oa = this->reducer_.GetOA(h, w);
              this->compressor_.InputRaw(h, w, oa);
              specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
              if (type == OA) this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
              else if (type == WEIGHT) this->mem_controller_.WriteZKernelToDram(oa_bundle);
              else if (type == SCORE) this->mem_controller_.WriteResultToDram(oa_bundle, oa_idx);
              PrintIABundle(oa_bundle, reducer_stream);
              oa_idx++;
            }
          }
        }
      }
      // finish one batch (num_of_set x num_of_pe_in_set)
      // check for output from reducer
      while (reducer_.output_ready) {
        int h = 0;
        int w = 0;
        std::vector<int> oa = this->reducer_.GetOA(h, w);
        this->compressor_.InputRaw(h, w, oa);
        specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
        if (type == OA) this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
        else if (type == WEIGHT) this->mem_controller_.WriteZKernelToDram(oa_bundle);
        else if (type == SCORE) this->mem_controller_.WriteResultToDram(oa_bundle, oa_idx);
        PrintIABundle(oa_bundle, reducer_stream);
        oa_idx++;
      }
    }
  }
  this->reducer_.AllPartialSumFinish();
  while(!this->reducer_.finish) {
    int h = 0;
    int w = 0;
    std::vector<int> oa = this->reducer_.GetOA(h, w);
    this->compressor_.InputRaw(h, w, oa);
    specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
    if (type == OA) this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
    else if (type == WEIGHT) this->mem_controller_.WriteZKernelToDram(oa_bundle);
    else if (type == SCORE) this->mem_controller_.WriteResultToDram(oa_bundle, oa_idx);
    PrintIABundle(oa_bundle, reducer_stream);
    oa_idx++;
  }
  // std::string IaSram_log_file_path = "IaSram.log";
  // std::ofstream IaSramfile;
  // IaSramfile.open(IaSram_log_file_path);
  // std::ostream& IaSram_stream = IaSramfile;
  // PrintIaSram(mem_controller_, IaSram_stream);
  // IaSramfile.close();

  // std::string WSram_log_file_path = "WSram.log";
  // std::ofstream WSramfile;
  // WSramfile.open(WSram_log_file_path);
  // std::ostream& WSram_stream = WSramfile;
  // PrintWSram(mem_controller_, WSram_stream);
  // WSramfile.close();
}

void Controller::ZK_Convolution(specs::ConvConfig conv_config, bool verbose) {
  std::ofstream ia_file;
  ia_file.open("ia.log");
  std::ostream& ia_stream = ia_file;
  
  std::ofstream reducer_output_file;
  reducer_output_file.open("reducer.log");
  std::ostream& reducer_stream = reducer_output_file;
  
  int ia_bundle_num = conv_config.num_of_ia_bundle;
  int w_bundle_num  = conv_config.num_of_w_bundle;

  // assume SRAM is big enough to save a whole bundle set
  // move weight bundle set from DRAM to SRAM
  if (verbose) std::cout << "Moving weight & ia from DRAM" << std::endl;
  mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(conv_config.ia_bundle_set_index);
  mem_controller_.MoveWeightBundleSetFromDramToWSram(conv_config.w_bundle_set_index);

  // config reducer, mem controller, PE
  if (verbose) std::cout << "Setting convolution config for reducer, memory controller, PEs" << std::endl;
  this->compressor_.SetOAChannel(conv_config.oa_channel);
  mem_controller_.InputCalibrate(conv_config.ia_bundle_set_index); // im_z_crop 127x127x3
  mem_controller_.OutputCalibrate(conv_config.oa_bundle_set_index);
  mem_controller_.WeightCalibrate(conv_config.w_bundle_set_index);
  for (int i = 0; i < this->num_of_PE_set_; i++) {
    for (int j = 0; j < this->num_of_PE_in_set_; j++) {
      this->PEs_[i][j].SetConvConfig(conv_config);
    }
  }
  // read bias for this layer
  if (verbose) std::cout << "Read bias" << std::endl;  
  std::vector<int> bias = mem_controller_.ReadWeightBiasFromWSram(conv_config.w_bundle_set_index);
  this->reducer_.SetConvConfig(conv_config, bias);

  // consider buffer(RF) for bundle
  // since SRAM will take too much time
  if (verbose) std::cout << "Start processing..." << std::endl;
  int ia_idx = 0; // ia bundle index
  int w_idx = 0;  // w bundle index
  int oa_idx = 0;
  int smallest_ia_h = 0;
  int smallest_ia_w = 0;
  std::vector<specs::InputActivationBundle> ia_bundle_batch;
  std::vector<specs::WeightBundle> w_bundle_batch;

  while (ia_idx < ia_bundle_num) {
    w_idx = 0;
    if (!(ia_idx%100)) std::cout << "Working on ia bundle " << ia_idx << "/" << ia_bundle_num << std::endl;
    ia_bundle_batch.clear();
    smallest_ia_h = ia_idx/conv_config.ia_size;
    smallest_ia_w = ia_idx%conv_config.ia_size;
    // read ia bundles out, input stationary
    for (int i = 0; i < this->num_of_PE_set_; i++) {
      if (ia_idx >= ia_bundle_num) break; // end of MAC operation
      specs::InputActivationBundle b = mem_controller_.ReadInputActivationBundleFromIaSram(ia_idx);
      PrintIABundle(b, ia_stream);
      ia_bundle_batch.push_back(b);
      ia_idx++;
    }

    // iterate over all weight bundles
    while (w_idx < w_bundle_num) {
      w_bundle_batch.clear();
      // read w bundles out
      for (int j = 0; j < this->num_of_PE_in_set_; j++) {
        if (w_idx >= w_bundle_num) break;
        specs::WeightBundle b = mem_controller_.ReadWeightBundleFromWSram(w_idx);
        w_bundle_batch.push_back(b);
        w_idx++;
      }
      for (int l = 0; l < w_bundle_batch.size(); l++) { // not iterate over num_of_PE_in_set for edge condition
        for (int k = 0; k < ia_bundle_batch.size(); k++) {
          // this for-loop execute in same cycle, over each aims and PEs
          // FIXME: replace pseudo AIM with real one
          // this->aims_[k].InputBundle(ia_bundle_batch[k], w_bundle_batch[l]);
          // std::vector<specs::ValidPositionPair> v_p = aims_[k].GetValidPositionPairs();
          std::vector<specs::ValidPositionPair> v_p = this->PseudoAIM(ia_bundle_batch[k], w_bundle_batch[l]);
          this->PEs_[k][l].InputIABundle(ia_bundle_batch[k]);
          this->PEs_[k][l].InputWBundle(w_bundle_batch[l]);
          this->PEs_[k][l].InputValidPosition(v_p);
          this->PEs_[k][l].Process();
          while (!this->PEs_[k][l].finish || this->PEs_[k][l].output_valid) {
            if (this->PEs_[k][l].output_valid) {
              specs::RawData psum = this->PEs_[k][l].GetPsum();
              this->reducer_.Reduce(psum, smallest_ia_h, smallest_ia_w);
            }
            else if (!this->PEs_[k][l].finish) {
              this->PEs_[k][l].Process();
            }
            // check for output from reducer
            while (this->reducer_.output_ready) {
              int h = 0;
              int w = 0;
              std::vector<int> oa = this->reducer_.GetOA(h, w);
              this->compressor_.InputRaw(h, w, oa);
              specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
              this->mem_controller_.WriteZKernelToDram(oa_bundle);
              PrintIABundle(oa_bundle, reducer_stream);
              oa_idx++;
            }
          }
        }
      }
      // finish one batch (num_of_set x num_of_pe_in_set)
      // check for output from reducer
      while (reducer_.output_ready) {
        int h = 0;
        int w = 0;
        std::vector<int> oa = this->reducer_.GetOA(h, w);
        this->compressor_.InputRaw(h, w, oa);
        specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
        this->mem_controller_.WriteZKernelToDram(oa_bundle);
        PrintIABundle(oa_bundle, reducer_stream);
        oa_idx++;
      }
    }
  }
  this->reducer_.AllPartialSumFinish();
  while(!this->reducer_.finish) {
    int h = 0;
    int w = 0;
    std::vector<int> oa = this->reducer_.GetOA(h, w);
    this->compressor_.InputRaw(h, w, oa);
    specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
    this->mem_controller_.WriteZKernelToDram(oa_bundle);
    PrintIABundle(oa_bundle, reducer_stream);
    oa_idx++;
  }
}

void Controller::XCorr(specs::ConvConfig conv_config, bool verbose) {
  std::ofstream reducer_output_file;
  reducer_output_file.open("reducer.log");
  std::ostream& reducer_stream = reducer_output_file;

  int ia_bundle_num = conv_config.num_of_ia_bundle;

  // assume SRAM is big enough to save a whole bundle set
  // move weight bundle set from DRAM to SRAM
  if (verbose) std::cout << "Moving weight & ia from DRAM" << std::endl;
  mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(conv_config.ia_bundle_set_index);
  mem_controller_.MoveWeightBundleSetFromDramToWSram(conv_config.w_bundle_set_index);


  // config reducer, mem controller, PE
  if (verbose) std::cout << "Setting convolution config for reducer, memory controller, PEs" << std::endl;
  this->compressor_.SetOAChannel(conv_config.oa_channel);
  mem_controller_.InputCalibrate(conv_config.ia_bundle_set_index); // im_z_crop 127x127x3
  mem_controller_.OutputCalibrate(conv_config.oa_bundle_set_index);
  mem_controller_.WeightCalibrate(conv_config.w_bundle_set_index);

  for (int i = 0; i < this->num_of_PE_set_; i++) {
    for (int j = 0; j < this->num_of_PE_in_set_; j++) {
      this->PEs_[i][j].SetConvConfig(conv_config);
    }
  }
  std::vector<int> bias(256,0);
  this->reducer_.SetConvConfig(conv_config, bias);

  // read all weight bundle
  if (verbose) std::cout << "read all weight bundle out..." << std::endl;
  std::vector<specs::WeightBundle> zkernels; 
  for (int s = 0; s < conv_config.kernel_size; s++){
    std::vector<specs::WeightBundle> tmp = this->mem_controller_.ReadZKernelBundleSetFromWSram(s);
    for (auto it : tmp) zkernels.push_back(it);
  }


  // std::string WBundle_log_file_path = "WBundle.log";
  // std::ofstream WBundlefile;
  // WBundlefile.open(WBundle_log_file_path);
  // std::ostream& WBundle_stream = WBundlefile;
  // for (auto b:zkernels){
  //   PrintWBundle(b, WBundle_stream);
  // }
  // WBundlefile.close();

  // consider buffer(RF) for bundle
  // since SRAM will take too much time
  if (verbose) std::cout << "Start processing..." << std::endl;
  int ia_idx = 0; // ia bundle index
  int w_idx = 0;  // w bundle index
  int oa_idx = 0;
  int smallest_ia_h = 0;
  int smallest_ia_w = 0;
  std::vector<specs::InputActivationBundle> ia_bundle_batch;
  std::vector<specs::WeightBundle> w_bundle_batch;

  while (ia_idx < ia_bundle_num) {
    w_idx = 0;
    if (!(ia_idx%100)) std::cout << "Working on ia bundle " << ia_idx << "/" << ia_bundle_num << std::endl;
    ia_bundle_batch.clear();
    smallest_ia_h = ia_idx/conv_config.ia_size;
    smallest_ia_w = ia_idx%conv_config.ia_size;
    // read ia bundles out, input stationary
    for (int i = 0; i < this->num_of_PE_set_; i++) {
      if (ia_idx >= ia_bundle_num) break; // end of MAC operation
      specs::InputActivationBundle b = mem_controller_.ReadInputActivationBundleFromIaSram(ia_idx);
      ia_bundle_batch.push_back(b);
      ia_idx++;
    }

    // iterate over all weight bundles
    while (w_idx < zkernels.size()) {
      w_bundle_batch.clear();
      // read w bundles out
      for (int j = 0; j < this->num_of_PE_in_set_; j++) {
        if (w_idx >= zkernels.size()) break;
        specs::WeightBundle b = zkernels[w_idx];
        w_bundle_batch.push_back(b);
        w_idx++;
      }
      for (int l = 0; l < w_bundle_batch.size(); l++) { // not iterate over num_of_PE_in_set for edge condition
        for (int k = 0; k < ia_bundle_batch.size(); k++) {
          // this for-loop execute in same cycle, over each aims and PEs
          // FIXME: replace pseudo AIM with real one
          // this->aims_[k].InputBundle(ia_bundle_batch[k], w_bundle_batch[l]);
          // std::vector<specs::ValidPositionPair> v_p = aims_[k].GetValidPositionPairs();
          std::vector<specs::ValidPositionPair> v_p = this->PseudoAIM(ia_bundle_batch[k], w_bundle_batch[l]);
          this->PEs_[k][l].InputIABundle(ia_bundle_batch[k]);
          this->PEs_[k][l].InputWBundle(w_bundle_batch[l]);
          this->PEs_[k][l].InputValidPosition(v_p);
          this->PEs_[k][l].Process();
          while (!this->PEs_[k][l].finish || this->PEs_[k][l].output_valid) {
            if (this->PEs_[k][l].output_valid) {
              specs::RawData psum = this->PEs_[k][l].GetPsum();
              this->reducer_.Reduce(psum, smallest_ia_h, smallest_ia_w);
            }
            else if (!this->PEs_[k][l].finish) {
              this->PEs_[k][l].Process();
            }
            // check for output from reducer
            while (this->reducer_.output_ready) {
              int h = 0;
              int w = 0;
              std::vector<int> oa = this->reducer_.GetOA(h, w);
              this->compressor_.InputRaw(h, w, oa);
              specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
              this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
              // PrintIABundle(oa_bundle, reducer_stream);
              oa_idx++;
            }
          }
        }
      }
      // finish one batch (num_of_set x num_of_pe_in_set)
      // check for output from reducer
      while (reducer_.output_ready) {
        int h = 0;
        int w = 0;
        std::vector<int> oa = this->reducer_.GetOA(h, w);
        this->compressor_.InputRaw(h, w, oa);
        specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
        this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
        oa_idx++;
      }
    }
  }
  this->reducer_.AllPartialSumFinish();
  while(!this->reducer_.finish) {
    int h = 0;
    int w = 0;
    std::vector<int> oa = this->reducer_.GetOA(h, w);
    this->compressor_.InputRaw(h, w, oa);
    specs::InputActivationBundle oa_bundle = this->compressor_.Compress();
    this->mem_controller_.WriteInputActivationBundleToDram(oa_bundle, oa_idx);
    // PrintIABundle(oa_bundle, reducer_stream);
    oa_idx++;
  }

  // std::string IaSram_log_file_path = "IaSram.log";
  // std::ofstream IaSramfile;
  // IaSramfile.open(IaSram_log_file_path);
  // std::ostream& IaSram_stream = IaSramfile;
  // PrintIaSram(mem_controller_, IaSram_stream);
  // IaSramfile.close();
}

void Controller::ReadOA(int bundle_set_index, int num_of_oa_bundle, std::string name) {
  std::string log_file_path = name + ".log";
  std::ofstream myfile;
  myfile.open(log_file_path);
  std::ostream& stream = myfile;

  mem_controller_.MoveInputActivationBundleSetFromDramToIaSram(bundle_set_index);
  this->mem_controller_.InputCalibrate(bundle_set_index);
  for (int i = 0; i < num_of_oa_bundle; i++){
    specs::InputActivationBundle ia_bundle = this->mem_controller_.ReadInputActivationBundleFromIaSram(i);
    PrintIABundle(ia_bundle, stream);
  }
}

void Controller::ReadWeight(int bundle_set_index, int num_of_w_bundle, std::string name) {
  std::string log_file_path = name + ".log";
  std::ofstream myfile;
  myfile.open(log_file_path);
  std::ostream& stream = myfile;

  mem_controller_.MoveWeightBundleSetFromDramToWSram(bundle_set_index);
  this->mem_controller_.WeightCalibrate(bundle_set_index);
  for (int i = 0; i < num_of_w_bundle; i++){
    specs::WeightBundle w_bundle = this->mem_controller_.ReadWeightBundleFromWSram(i);
    PrintWBundle(w_bundle, stream);
  }
}

void Controller::ReadZKernel(int bundle_set_index, int kernel_size, std::string name) {
  std::string log_file_path = name + ".log";
  std::ofstream myfile;
  myfile.open(log_file_path);
  std::ostream& stream = myfile;

  mem_controller_.MoveWeightBundleSetFromDramToWSram(bundle_set_index);
  this->mem_controller_.WeightCalibrate(bundle_set_index);
  int i = 0;
  for (int s = 0; s < kernel_size; s++){
    std::vector<specs::WeightBundle> tmp = this->mem_controller_.ReadZKernelBundleSetFromWSram(s);
    for (auto it : tmp) { 
      PrintWBundle(it, stream);
      i++;
      // std::cout << "bundle count: " << i << std::endl;
    }
  }
}

void Controller::InitCompressInput(specs::RawData raw_data) {
  // // FIXME: write bundle might need to fix
  // // read raw_data to compressor
  // this->compressor_.GetRaw(raw_data);
  // // compress input target image, 127x127x3
  // specs::InputActivationBundleSet ia_bundle_set = this->compressor_.Compress();
  // // write bundle set to IA SRAM
  // int num_of_bundle = ALEXNET_CONV1.num_of_ia_bundle;
  // // write bundle into SRAM
  // // assume SRAM can hold all bundles
  // for (int i = 0; i < num_of_bundle; i++) {
  //   mem_controller_.WriteInputActivationBundleToDram(ia_bundle_set.input_activation_bundles[i], i);
  // }
}

