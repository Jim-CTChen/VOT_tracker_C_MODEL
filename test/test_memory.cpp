#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include "../inc/memory_controller.h"
#include "../inc/specs.h"
// #include "../inc/address_config.h"
#include "../inc/address_config_v1.h"

using namespace std;

class TestMemoryController{
  public:
    TestMemoryController(){};

    MemoryController mem_ctrl = MemoryController();

    void InitializeDramImg();
    void InitializeDramRandomOA(int output_ft_map_idx);
    void InitializeDramOA(string file_name);
    void InitializeDramWeight(string file_name);
    void PrintDram(ostream& stream);
    void PrintIaSram(ostream& stream);
    void PrintWSram(ostream& stream);
};

void TestMemoryController::InitializeDramImg(){
  int dep_max = 3;
  int dep_min = 0;
  int max = 256;
  int min = 0;
  int x;
  vector<int> temp(kDramAccessBytes,0);
  //img_z
  for (int row=kimg_z_addr_depth_start; row<kimg_z_addr_depth_end; row++){
    for (int i=0; i<kDramAccessBytes; i++){
      temp[i] = rand()%(dep_max-dep_min+1) + min;
    }
    this->mem_ctrl.dram_.mem_[row] = temp;
  }
  for (int row=kimg_z_addr_data_start; row<kimg_z_addr_end; row++){
    for (int i=0; i<kDramAccessBytes; i++){
      temp[i] = rand()%(max-min+1) + min;
    }
    this->mem_ctrl.dram_.mem_[row] = temp;
  }
  //img_x
  for (int row=kimg_x_addr_depth_start; row<kimg_x_addr_depth_end; row++){
    for (int i=0; i<kDramAccessBytes; i++){
      temp[i] = rand()%(dep_max-dep_min+1) + min;
    }
    this->mem_ctrl.dram_.mem_[row] = temp;
  }
  for (int row=kimg_x_addr_data_start; row<kimg_x_addr_end; row++){
    for (int i=0; i<kDramAccessBytes; i++){
      temp[i] = rand()%(max-min+1) + min;
    }
    this->mem_ctrl.dram_.mem_[row] = temp;
  }
}

// e.g. out_ft_map=1
void TestMemoryController::InitializeDramRandomOA(int output_ft_map_idx){
  int dep_max;
  int addr_data_end;
  switch (output_ft_map_idx){
    case(1):
      dep_max = 96;
      addr_data_end = koa_ft_map_addr_data_end_1;
      break;
    case(18):
      dep_max = 96;
      addr_data_end = koa_ft_map_addr_data_end_18;
      break;
    case(19):
      dep_max = 128;
      addr_data_end = koa_ft_map_addr_data_end_19;
      break;
    default:
      dep_max = 96;
      addr_data_end = koa_ft_map_addr_data_end_1;
      break;
  }
  int dep_min = 0;
  int max = 256;
  int min = 0;
  vector<int> temp(kDramAccessBytes,0);

  //oa, take conv1 for example
  this->mem_ctrl.addr_dram_generator_.CalibrateWriteAddress(output_ft_map_idx);
  int addr_data_start = this->mem_ctrl.addr_dram_generator_.addr_data_start;
  // int addr_data_end = koa_ft_map_addr_data_end_1;
  // assert(addr_data_start==koa_ft_map_addr_data_start_1);
  // assert(addr_data_end==koa_ft_map_addr_data_end_1);

  for (int row=koa_ft_map_addr_depth_start; row<addr_data_start; row++){
    for (int i=0; i<kDramAccessBytes; i++){
      temp[i] = rand()%(dep_max-dep_min+1) + dep_min;
    }
    this->mem_ctrl.dram_.mem_[row] = temp;
  }
  for (int row=addr_data_start; row<addr_data_end; row++){
    for (int i=0; i<kDramAccessBytes; i++){
      temp[i] = rand()%(max-min+1) + min;
    }
    this->mem_ctrl.dram_.mem_[row] = temp;
  }
}

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

void TestMemoryController::InitializeDramOA(string file_name){
  std::vector<std::string> temp_s;
  std::ifstream ia_file;
  std::string s;
  std::vector<int> temp(kDramAccessBytes,0);
  ia_file.open(file_name);
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

    this->mem_ctrl.dram_.mem_[address] = temp;
    std::fill(temp.begin(), temp.end(), 0);
  }
  cout << "Write ia to Dram Finish"<<endl;

  ia_file.close();
}

void TestMemoryController::InitializeDramWeight(string file_name){
  std::vector<std::string> temp_s;
  std::ifstream weight_file;
  std::string s;
  std::vector<int> temp(kDramAccessBytes,0);
  weight_file.open(file_name);
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

    this->mem_ctrl.dram_.mem_[address] = temp;
    std::fill(temp.begin(), temp.end(), 0);
  }
  cout << "Write weight to Dram Finish"<<endl;

  weight_file.close();
}





void TestMemoryController::PrintDram(ostream& stream){
  stream << "Print Dram" << endl;
  for (int row=0; row<this->mem_ctrl.dram_.mem_.size(); row++){
    //img_z
    if (row==kimg_z_addr_depth_start){
      stream << "\n===========   kimg_z_addr_depth_start  ===========\n\n";}
    if (row==kimg_z_addr_depth_end){
      stream << "\n===========   kimg_z_addr_depth_end ==============\n\n";}
    if (row==kimg_z_addr_data_start){
      stream << "\n===========   kimg_z_addr_data_start  ============\n\n";
      stream << "\n===========   interval of data: "<<kimg_z_addr_interval_of_data<<"  ===========\n\n";}
    if (row==kimg_z_addr_end){
      stream << "\n===========   kimg_z_addr_end  ===================\n\n";}
    //img_x
    if (row==kimg_x_addr_depth_start){
      stream << "\n===========   kimg_x_addr_depth_start  ===========\n\n";}
    if (row==kimg_x_addr_depth_end){
      stream << "\n===========   kimg_x_addr_depth_end ==============\n\n";}
    if (row==kimg_x_addr_data_start){
      stream << "\n===========   kimg_x_addr_data_start  ============\n\n";
      stream << "\n===========   interval of data: "<<kimg_x_addr_interval_of_data<<"  ===========\n\n";
    }
    if (row==kimg_x_addr_end){
      stream << "\n===========   kimg_x_addr_end  ===================\n\n";
    }
    //oa
    if (row==koa_ft_map_addr_depth_start){
      stream << "\n===========   koa_ft_map_addr_depth_start  ===========\n\n";
    }
    if (row==this->mem_ctrl.addr_dram_generator_.addr_data_start){
      stream << "\n===========   addr_dram_generator_.addr_data_start  ============\n\n";
      stream << "\n===========   addr_dram_generator_.interval_of_data: "<<this->mem_ctrl.addr_dram_generator_.addr_interval_of_data<<"  ===========\n\n";
    }
    if (row==koa_ft_map_addr_end){
      stream << "\n===========   koa_ft_map_addr_end  ===================\n\n";
    }
    //Result
    if (row==kresult_cls_score_start){
      stream << "\n===========   kresult_cls_score_start  ===================\n\n";
    }
    if (row==kresult_cls_score_end){
      stream << "\n===========   kresult_cls_score_end  ===================\n\n";
    }
    if (row==kresult_ctr_score_start){
      stream << "\n===========   kresult_ctr_score_start  ===================\n\n";
    }
    if (row==kresult_ctr_score_end){
      stream << "\n===========   kresult_ctr_score_end  ===================\n\n";
    }
    if (row==kresult_offset_score_start){
      stream << "\n===========   kresult_offset_score_start  ===================\n\n";
    }
    if (row==kresult_offset_score_end){
      stream << "\n===========   kresult_offset_score_end  ===================\n\n";
    }
    //c_z
    if (row==kc_z_addr_depth_start){
      stream << "\n===========   kc_z_addr_depth_start  ===================\n\n";
    }
    if (row==kc_z_addr_depth_end){
      stream << "\n===========   kc_z_addr_depth_end  ===================\n\n";
    }
    if (row==kc_z_addr_data_start){
      stream << "\n===========   kc_z_addr_data_start  ===================\n\n";
    }
    if (row==kc_z_addr_end){
      stream << "\n===========   kc_z_addr_end    ===================\n\n";
    }
    //r_z
    if (row==kr_z_addr_depth_start){
      stream << "\n===========   kr_z_addr_depth_start  ===================\n\n";
    }
    if (row==kr_z_addr_depth_end){
      stream << "\n===========   kr_z_addr_depth_end  ===================\n\n";
    }
    if (row==kr_z_addr_data_start){
      stream << "\n===========   kr_z_addr_data_start  ===================\n\n";
    }
    if (row==kr_z_addr_end){
      stream << "\n===========   kr_z_addr_end    ===================\n\n";
    }
    //conv1
    if (row==kconv1_addr_s_start){
      stream << "\n===========   kconv1_addr_s_start  ===================\n\n";
    }
    if (row==kconv1_addr_s_end){
      stream << "\n===========   kconv1_addr_s_end  ===================\n\n";
    }
    if (row==kconv1_addr_depth_start){
      stream << "\n===========   kconv1_addr_depth_start  ===================\n\n";
    }
    if (row==kconv1_addr_depth_end){
      stream << "\n===========   kconv1_addr_depth_end  ===================\n\n";
    }
    if (row==kconv1_addr_prkdepth_start){
      stream << "\n===========   kconv1_addr_prkdepth_start  ===================\n\n";
    }
    if (row==kconv1_addr_prkdepth_end){
      stream << "\n===========   kconv1_addr_prkdepth_end  ===================\n\n";
    }
    if (row==kconv1_addr_data_start){
      stream << "\n===========   kconv1_addr_data_start  ===================\n\n";
      stream << "\n===========   interval of data: "<<kconv1_addr_interval_of_data<<"  ===================\n\n";
    }
    if (row==kcls_conv1_addr_data_end){
      stream << "\n===========   kcls_conv1_addr_data_end  ===================\n\n";
    }
    if (row==kconv1_addr_prk_start){
      stream << "\n===========   kconv1_addr_prk_start  ===================\n\n";
      stream << "\n===========   interval of prk: "<<kconv1_addr_interval_of_prk<<"  ===================\n\n";
    }
    if (row==kcls_conv1_addr_prk_end){
      stream << "\n===========   kcls_conv1_addr_prk_end  ===================\n\n";
    }


    stream << row << "\t[  ";
    for (int col=0; col<kDramAccessBytes; col++){
      stream << this->mem_ctrl.dram_.mem_[row][col] << "  ";
    }
    stream << "\t]\n";
  }
}

void TestMemoryController::PrintIaSram(ostream& stream){
  stream << "Print IaSram" << endl;
  for (int row=0; row<this->mem_ctrl.ia_sram_.mem_.size(); row++){
    if (row==this->mem_ctrl.addr_ia_sram_generator_.addr_depth_start){
      stream << "\n===========   addr_depth_start  ===========\n\n";
    }
    if (row==this->mem_ctrl.addr_ia_sram_generator_.addr_data_start){
      stream << "\n===========   addr_data_start  ============\n\n";
      stream << "\n===========   interval of data: "<<this->mem_ctrl.addr_ia_sram_generator_.addr_interval_of_data<< "  ============\n\n";
    }
    stream << row << "\t[  ";
    for (int col=0; col<kSramAccessBytes; col++){
      stream << this->mem_ctrl.ia_sram_.mem_[row][col] << "  ";
    }
    stream << "\t]\n";
  }
}

void TestMemoryController::PrintWSram(ostream& stream){
  stream << "Print WSram" << endl;
  std::cout<< "WSram size: "<<this->mem_ctrl.w_sram_.mem_.size()<<std::endl;
  for (int row=0; row<this->mem_ctrl.w_sram_.mem_.size(); row++){
    if (row==this->mem_ctrl.addr_w_sram_generator_.addr_s_start){
      stream << "\n===========   addr_s_start  ===========\n\n";
    }
    if (row==this->mem_ctrl.addr_w_sram_generator_.addr_depth_start){
      stream << "\n===========   addr_depth_start  ===========\n\n";
    }
    if (row==this->mem_ctrl.addr_w_sram_generator_.addr_prk_depth_start){
      stream << "\n===========   addr_prk_depth_start  ===========\n\n";
    }
    if (row==this->mem_ctrl.addr_w_sram_generator_.addr_data_start){
      stream << "\n===========   addr_data_start  ===========\n\n";
      stream << "\n===========   interval of data: "<<this->mem_ctrl.addr_w_sram_generator_.addr_interval_of_data<<"  ===========\n\n";
    }
    if (row==this->mem_ctrl.addr_w_sram_generator_.addr_prk_start){
      stream << "\n===========   addr_prk_start  ===========\n\n";
      stream << "\n===========   interval of prk: "<<this->mem_ctrl.addr_w_sram_generator_.addr_interval_of_prk<<"  ===========\n\n";
    }

    stream << row << "\t[  ";
    for (int col=0; col<kSramAccessBytes; col++){
      stream << this->mem_ctrl.w_sram_.mem_[row][col] << "  ";
    }
    stream << "\t]\n";
  }
}

void PrintIABundle(specs::InputActivationBundle b, ostream& stream = cout) {
  // assert(b.data.size() == b.depth);
  // assert(b.channel_idx.size() == b.size);

  stream << "===== bundle info =====" << endl;
  stream << "\t" << "(h, w): (" << b.h  << ", " << b.w << ")" << endl;
  // stream << "\t" << "size: " << b.size << endl;
  stream << "\t" << "depth: " << b.depth << endl;
  for (int i = 0; i < b.data.size(); i++) {
    stream << "  [" << i << "]  " << "<data, channel>: <" << b.data[i] << ", " << b.channel_idx[i] << ">" << endl;
  }
  stream << "======================" << endl;
}

void PrintWBundle(specs::WeightBundle b, ostream& stream = cout) {
  // assert(b.data.size() == b.depth);
  // assert(b.channel_idx.size() == b.size);

  stream << "===== bundle info =====" << endl;
  stream << "\t" << "(s, dep, prkdepth): (" << b.s<< ", " << b.depth << ", " << b.prk_depth << ")" << endl;
  for (int i = 0; i < b.data.size(); i++) {
    stream << "  [" << i << "]  " << "<data, channel>: <" << b.data[i] << ", " << b.channel_idx[i] << ">" << endl;
  }
  for (int i = 0; i < b.pos_ptr.size(); i++) {
    stream << "  [" << i << "]  " << "<prk>: <" << b.pos_ptr[i] << ", " << b.r_idx[i] << ", "<<b.k_idx[i]<< ">" << endl;
  }
  stream << "======================" << endl;
}

void PrintBias(std::vector<int> bias, ostream& stream=cout){
  stream << "===== bias length: "<<bias.size() <<"  ====="<< endl;
  for (int i=0; i<bias.size();i++){
    stream << "  [" << i << "]  " << "<data>: <" << bias[i] << ">" << endl;
  }
}

specs::InputActivationBundle GenerateIaBundle(int h, int w, int size){
  specs::InputActivationBundle bundle;
  int dep_max = 96-1;
  int dep_min = 0;
  int max = 255;
  int min = 0;

  int depth = rand()%(dep_max-dep_min+1) + min;
  vector<int> temp(depth,0);
  for (int i=0; i<depth; i++){
    temp[i] = rand()%(max-min+1) + min;
  }
  bundle.data = temp;
  for (int i=0; i<depth; i++){
    temp[i] = rand()%(max-min+1) + min;
  }
  sort(temp.begin(),temp.end());
  bundle.channel_idx = temp;
  bundle.h = h;
  bundle.w = w;
  bundle.depth = depth;
  return bundle;
}

vector<specs::InputActivationBundle> GenerateIaBundleSet(int output_ft_map_idx){
  vector<specs::InputActivationBundle> bundleset;
  int dep_max;
  int size;
  switch(output_ft_map_idx){
    case(1):{
      dep_max = 96;
      size = 63;
      break;
    }
    case(9):{
      dep_max = 256;
      size = 6;
      break;
    }
    case(14):{
      dep_max = 256;
      size = 6;
      break;
    }
    case(18):{
      dep_max = 96;
      size = 149;
      break;
    }
    case(19):{
      dep_max = 128;
      size = 149;
      break;
    }
    case(36):{
      dep_max = 1;
      size = 17;
      break;
    }
    case(42):{
      dep_max = 4;
      size = 17;
      break;
    }
  }
  specs::InputActivationBundle bundle;
  vector<int> temp;
  int dep_min = 0;
  int max = 255;
  int min = 0;
  int depth;

  for (int idx=0; idx<size*size; idx++){
    depth = rand()%(dep_max-dep_min+1) + dep_min;
    temp = vector<int>(depth,0);
    for (int i=0; i<depth; i++){
      temp[i] = rand()%(max-min+1) + min;
    }
    bundle.data = temp;
    for (int i=0; i<depth; i++){
      temp[i] = rand()%(max-min+1) + min;
    }
    sort(temp.begin(),temp.end());
    bundle.channel_idx = temp;

    bundle.h = idx/size;
    bundle.w = idx%size;
    bundle.depth = depth;
    bundleset.push_back(bundle);
  }
  return bundleset;
}


int main() {
  // logging
  srand(0);
  std::string dram_path = "dram.log";
  std::string ia_sram_path = "ia_sram.log";
  std::string w_sram_path = "w_sram.log";
  ofstream dramfile;
  ofstream ia_sramfile;
  ofstream w_sramfile;
  dramfile.open(dram_path);
  ia_sramfile.open(ia_sram_path);
  w_sramfile.open(w_sram_path);
  ostream& dram_stream = dramfile;
  ostream& ia_sram_stream = ia_sramfile;
  ostream& w_sram_stream = w_sramfile;

  specs::InputActivationBundle bundle;
  specs::WeightBundle wbundle;
  std::vector<int> bias;
  vector<specs::InputActivationBundle> bundleset;

  TestMemoryController tb;
  int input_ft_map_idx;
  int output_ft_map_idx;
  cout << "Initializing Memory..." << endl;
  // tb.InitializeDramImg();
  tb.InitializeDramWeight("./test/test_conv_bias.txt");
  tb.InitializeDramOA("./test/test_input.txt");
  cout << "Initializing Finish" << endl;


  // //Move img_z from Dram to Sram
  // tb.mem_ctrl.MoveInputActivationBundleSetFromDramToIaSram(0);
  // cout << "Move ia bundle from Dram to Sram Finish" << endl;
  // //Read bundle
  // tb.mem_ctrl.InputCalibrate(0); //Input now is img_z
  // bundle = tb.mem_ctrl.ReadInputActivationBundleFromIaSram(5);
  // cout << "Read ia bundle from Sram Finish" << endl;
  // PrintIABundle(bundle);
  // bundle = tb.mem_ctrl.ReadInputActivationBundleFromIaSram(127*127-2);
  // cout << "Read ia bundle from Sram Finish" << endl;
  // PrintIABundle(bundle);
  // bundle = tb.mem_ctrl.ReadInputActivationBundleFromIaSram(127*127-1);
  // cout << "Read ia bundle from Sram Finish" << endl;
  // PrintIABundle(bundle);
  // tb.PrintIaSram(ia_sram_stream);


  // Move ia_bundle_set from Dram to Sram and read ia_bundle
  // e.g. input_ft_map_idx=...
  // input_ft_map_idx = 100;
  // output_ft_map_idx = 101;
  // // tb.InitializeDramRandomOA(input_ft_map_idx);
  // tb.mem_ctrl.MoveInputActivationBundleSetFromDramToIaSram(input_ft_map_idx);
  // cout << "Move ia bundle from Dram to Sram Finish" << endl;
  // //Read 0-th bundle
  // tb.mem_ctrl.InputCalibrate(input_ft_map_idx); //Input now is...
  // cout << "Input feature map size: " << tb.mem_ctrl.input_bundle_size <<endl;
  // bundle = tb.mem_ctrl.ReadInputActivationBundleFromIaSram(0);
  // cout << "Read ia bundle from Sram Finish" << endl;
  // PrintIABundle(bundle);
  // bundle = tb.mem_ctrl.ReadInputActivationBundleFromIaSram(1);
  // cout << "Read ia bundle from Sram Finish" << endl;
  // PrintIABundle(bundle);
  // tb.PrintIaSram(ia_sram_stream);


  // //Move weight from Dram to Sram and Read weight bundle
  // int weight_idx = 100;
  // tb.mem_ctrl.MoveWeightBundleSetFromDramToWSram(weight_idx);
  // cout<< "Move weight from Dram to Sram Finish" <<endl;
  // int wbundle_idx = 0;
  // tb.mem_ctrl.WeightCalibrate(weight_idx); //Input now is img_z
  // cout<< "Read weight, bias from Sram" <<endl;
  // wbundle = tb.mem_ctrl.ReadWeightBundleFromWSram(wbundle_idx);
  // bias = tb.mem_ctrl.ReadWeightBiasFromWSram(weight_idx);
  // PrintWBundle(wbundle);
  // PrintBias(bias);
  // tb.PrintWSram(w_sram_stream);
  


  // // Write output_ft_map_idx's bundle back to Dram
  // bundleset = GenerateIaBundleSet(9);
  // cout << "New bundle" <<endl;
  // tb.mem_ctrl.OutputCalibrate(9);
  // cout << "Output feature map size: " << tb.mem_ctrl.output_bundle_size <<endl;
  // PrintIABundle(bundleset[0]);
  // // PrintIABundle(bundleset[1]);
  // // PrintIABundle(bundleset[bundleset.size()-1]);
  // for (int i=0; i<bundleset.size(); i++){
  //   tb.mem_ctrl.WriteInputActivationBundleToDram(bundleset[i],i);
  // }
  // input_ft_map_idx = 9;
  // tb.mem_ctrl.MoveInputActivationBundleSetFromDramToIaSram(input_ft_map_idx);
  // cout << "Move ia bundle from Dram to Sram Finish" << endl;
  // tb.mem_ctrl.InputCalibrate(input_ft_map_idx); //Input now is...
  // cout << "Input feature map size: " << tb.mem_ctrl.input_bundle_size <<endl;
  // bundle = tb.mem_ctrl.ReadInputActivationBundleFromIaSram(0);
  // cout << "Read ia bundle from Sram Finish" << endl;
  // PrintIABundle(bundle);
  // tb.PrintIaSram(ia_sram_stream);

  // Write zkernel's bundle  to Dram
  bundleset = GenerateIaBundleSet(9);
  cout << "New bundle" <<endl;
  tb.mem_ctrl.OutputCalibrate(9);
  cout << "Output feature map size: " << tb.mem_ctrl.output_bundle_size <<endl;
  // PrintIABundle(bundleset[0]);
  // PrintIABundle(bundleset[6]);
  // PrintIABundle(bundleset[bundleset.size()-1]);
  for (int i=0; i<bundleset.size(); i++){
    tb.mem_ctrl.WriteZKernelToDram(bundleset[i]);
  }
  tb.PrintDram(dram_stream);

  // Move Z Kernel to Sram and Read
  int weight_idx = 40; //c_z
  tb.mem_ctrl.MoveWeightBundleSetFromDramToWSram(weight_idx);
  int s = 0;
  std::vector<specs::WeightBundle> zkernels;
  tb.mem_ctrl.WeightCalibrate(weight_idx);
  zkernels = tb.mem_ctrl.ReadZKernelBundleSetFromWSram(s);
  std::cout<<"zkdernels size is " <<zkernels.size()<<std::endl;
  for (int i=0; i<zkernels.size(); i++){
    PrintWBundle(zkernels[i]);
  }
  tb.PrintWSram(w_sram_stream);


  // // Write result bundle to Dram
  // bundleset = GenerateIaBundleSet(36); //offset
  // cout << "New bundle" <<endl;
  // tb.mem_ctrl.OutputCalibrate(36);
  // cout << "Output feature map size: " << tb.mem_ctrl.output_bundle_size <<endl;
  // PrintIABundle(bundleset[0]);
  // PrintIABundle(bundleset[1]);
  // PrintIABundle(bundleset[2]);
  // PrintIABundle(bundleset[bundleset.size()-1]);
  // for (int i=0; i<bundleset.size(); i++){
  //   tb.mem_ctrl.WriteResultToDram(bundleset[i],i);
  // }

  tb.PrintDram(dram_stream);

  dramfile.close();
  ia_sramfile.close();
  w_sramfile.close();
}