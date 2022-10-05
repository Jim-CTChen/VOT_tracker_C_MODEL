#include "tool.h"

#include <iostream>
#include <fstream>

#include "../inc/specs.h"
#include "../inc/address_config_v1.h"

// backbone alexnet
specs::ConvConfig    ALEXNET_IMGZ_CONV1;
specs::ConvConfig    ALEXNET_IMGZ_CONV2;
specs::MaxPoolConfig ALEXNET_IMGZ_MAXPOOL1;
specs::ConvConfig    ALEXNET_IMGZ_CONV3;
specs::MaxPoolConfig ALEXNET_IMGZ_MAXPOOL2;
specs::ConvConfig    ALEXNET_IMGZ_CONV4;
specs::ConvConfig    ALEXNET_IMGZ_CONV5;
specs::ConvConfig    ALEXNET_IMGZ_CONV6;

specs::ConvConfig    ALEXNET_IMGX_CONV1;
specs::ConvConfig    ALEXNET_IMGX_CONV2;
specs::MaxPoolConfig ALEXNET_IMGX_MAXPOOL1;
specs::ConvConfig    ALEXNET_IMGX_CONV3;
specs::MaxPoolConfig ALEXNET_IMGX_MAXPOOL2;
specs::ConvConfig    ALEXNET_IMGX_CONV4;
specs::ConvConfig    ALEXNET_IMGX_CONV5;
specs::ConvConfig    ALEXNET_IMGX_CONV6;

// init
specs::ConvConfig C_Z_K;
specs::ConvConfig R_Z_K;

// xcoor
specs::ConvConfig C_Z;
specs::ConvConfig R_Z;

// track
// cls & ctr branch
specs::ConvConfig C_X;
specs::ConvConfig C_CONV1;
specs::ConvConfig C_CONV2;
specs::ConvConfig C_CONV3;
specs::ConvConfig C_CLS_CONV;
specs::ConvConfig C_CTR_CONV;

// reg branch
specs::ConvConfig R_X;
specs::ConvConfig R_CONV1;
specs::ConvConfig R_CONV2;
specs::ConvConfig R_CONV3;
specs::ConvConfig R_REG_CONV;

specs::ConvConfig TEST_CONV;
specs::ConvConfig TEST_CONV_S2;



void PrintIaSram(MemoryController mem_ctrl, std::ostream& stream){
  stream << "Print IaSram" << std::endl;
  for (int row=0; row<mem_ctrl.ia_sram_.mem_.size(); row++){
    if (row==mem_ctrl.addr_ia_sram_generator_.addr_depth_start){
      stream << "\n===========   addr_depth_start  ===========\n\n";
    }
    if (row==mem_ctrl.addr_ia_sram_generator_.addr_data_start){
      stream << "\n===========   addr_data_start  ============\n\n";
      stream << "\n===========   interval of data: "<<mem_ctrl.addr_ia_sram_generator_.addr_interval_of_data<< "  ============\n\n";
    }
    stream << row << "\t[  ";
    for (int col=0; col<kSramAccessBytes; col++){
      stream << mem_ctrl.ia_sram_.mem_[row][col] << "  ";
    }
    stream << "\t]\n";
  }
}

void PrintWSram(MemoryController mem_ctrl, std::ostream& stream){
  stream << "Print WSram" << std::endl;
  std::cout<< "WSram size: "<<mem_ctrl.w_sram_.mem_.size()<<std::endl;
  for (int row=0; row<mem_ctrl.w_sram_.mem_.size(); row++){
    if (row==mem_ctrl.addr_w_sram_generator_.addr_s_start){
      stream << "\n===========   addr_s_start  ===========\n\n";
    }
    if (row==mem_ctrl.addr_w_sram_generator_.addr_depth_start){
      stream << "\n===========   addr_depth_start  ===========\n\n";
    }
    if (row==mem_ctrl.addr_w_sram_generator_.addr_prk_depth_start){
      stream << "\n===========   addr_prk_depth_start  ===========\n\n";
    }
    if (row==mem_ctrl.addr_w_sram_generator_.addr_data_start){
      stream << "\n===========   addr_data_start  ===========\n\n";
      stream << "\n===========   interval of data: "<<mem_ctrl.addr_w_sram_generator_.addr_interval_of_data<<"  ===========\n\n";
    }
    if (row==mem_ctrl.addr_w_sram_generator_.addr_prk_start){
      stream << "\n===========   addr_prk_start  ===========\n\n";
      stream << "\n===========   interval of prk: "<<mem_ctrl.addr_w_sram_generator_.addr_interval_of_prk<<"  ===========\n\n";
    }

    stream << row << "\t[  ";
    for (int col=0; col<kSramAccessBytes; col++){
      stream << mem_ctrl.w_sram_.mem_[row][col] << "  ";
    }
    stream << "\t]\n";
  }
}


void PrintDram(MemoryController mem_ctrl, std::ostream& stream){
  stream << "Print Dram" << std::endl;
  for (int row=0; row<mem_ctrl.dram_.mem_.size(); row++){
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
    if (row==mem_ctrl.addr_dram_generator_.addr_data_start){
      stream << "\n===========   addr_dram_generator_.addr_data_start  ============\n\n";
      stream << "\n===========   addr_dram_generator_.interval_of_data: "<<mem_ctrl.addr_dram_generator_.addr_interval_of_data<<"  ===========\n\n";
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


    stream << row << "  [  ";
    for (int col=0; col<kDramAccessBytes; col++){
      stream << mem_ctrl.dram_.mem_[row][col] << "  ";
    }
    stream << "]\n";
  }
}

void PrintDramResult(MemoryController mem_ctrl, std::ostream& stream){
  for (int row=kresult_cls_score_start; row<kresult_offset_score_end; row++){
    //img_z
    if (row==kresult_cls_score_start){
      stream << "\n===========   koa_ft_map_cls  ===========\n\n";}
    if (row==kresult_cls_score_end){
      stream << "\n===========   kresult_cls_score_end ==============\n\n";}
    if (row==kresult_ctr_score_start){
      stream << "\n===========   kresult_ctr_score_start ==============\n\n";}
    if (row==kresult_ctr_score_end){
      stream << "\n===========   kresult_ctr_score_end ==============\n\n";}
    if (row==kresult_offset_score_start){
      stream << "\n===========   kresult_offset_score_start ==============\n\n";}
    if (row==kresult_offset_score_end){
      stream << "\n===========   kresult_offset_score_end ==============\n\n";}

    stream << row << "  [  ";
    for (int col=0; col<kDramAccessBytes; col++){
      stream << mem_ctrl.dram_.mem_[row][col] << "  ";
    }
    stream << "]\n";
  }
}


void PrintBias(std::vector<int> bias, std::ostream& stream){
  stream << "===== bias length: "<<bias.size() <<"  ====="<< std::endl;
  for (int i=0; i<bias.size();i++){
    stream << "  [" << i << "]  " << "<data>: <" << bias[i] << ">" << std::endl;
  }
}

void PrintIABundle(specs::InputActivationBundle b, std::ostream& stream) {
  assert(b.data.size() == b.depth);
  assert(b.channel_idx.size() == b.depth);

  stream << "===== IA bundle info =====" << std::endl;
  stream << "\t" << "(h, w): (" << b.h  << ", " << b.w << ")" << std::endl;
  stream << "\t" << "depth: " << b.depth << std::endl;
  for (int i = 0; i < b.depth; i++) {
    stream << "\t" << "<data, channel>: <" << b.data[i] << ", " << b.channel_idx[i] << ">" << std::endl;
  }
  stream << "======================" << std::endl;
}

void PrintWBundle(specs::WeightBundle b, std::ostream& stream) {
  // std::cout << "size: "<< b.data.size() << std::endl;
  // std::cout << "channel size: " << b.channel_idx.size() << std::endl;
  // std::cout << "depth: " << b.depth << std::endl;
  assert(b.data.size() == b.depth);
  assert(b.channel_idx.size() == b.depth);

  stream << "===== W bundle info =====" << std::endl;
  stream << "\t" << "(s): (" << b.s << ")" << std::endl;
  stream << "\t" << "depth: " << b.depth << std::endl;
  stream << "\t" << "prk depth: " << b.prk_depth << std::endl;
  for (int i = 0; i < b.depth; i++) {
    stream << "\t" << "<data, channel>: <" << b.data[i] << ", " << b.channel_idx[i] << ">" << std::endl;
  }
  for (int i = 0; i < b.prk_depth; i++) {
    stream << "\t" << "<p, r, k>: <" << b.pos_ptr[i] << ", " << b.r_idx[i] << ", " << b.k_idx[i] << ">" << std::endl;
  }
  stream << "======================" << std::endl;
}

void PrintPartialSum(specs::RawData raw, std::ostream& stream) {
  int h, w, k, v;
  for (int i = 0; i < raw.h.size(); i++) {
    h = raw.h[i];
    w = raw.w[i];
    k = raw.k[i];
    v = raw.value[i];
    stream << "(h, w, k, value): " << "(" << h << ", " << w << ", " << k << ", " << v << ")" << std::endl;
  }
}

void PrintValidPositionPairs(std::vector<specs::ValidPositionPair> v_p, std::ostream& stream) {
  stream << "Printing Valid Position" << std::endl;
  for (int i = 0; i < v_p.size(); i++) {
    stream << "<valid, pos>: <" << v_p[i].valid << ", " << v_p[i].pos << ">" << std::endl;
  }
}

void InitConvConfig() {
  // IMGZ
  ALEXNET_IMGZ_CONV1.relu = true;
  ALEXNET_IMGZ_CONV1.kernel_size = 3;
  ALEXNET_IMGZ_CONV1.stride = 2;
  ALEXNET_IMGZ_CONV1.ia_size = 127;
  ALEXNET_IMGZ_CONV1.ia_channel = 3;
  ALEXNET_IMGZ_CONV1.oa_size = 63;
  ALEXNET_IMGZ_CONV1.oa_channel = 96;
  ALEXNET_IMGZ_CONV1.w_bundle_set_index = 1;
  ALEXNET_IMGZ_CONV1.ia_bundle_set_index = 0;
  ALEXNET_IMGZ_CONV1.oa_bundle_set_index = 1;
  ALEXNET_IMGZ_CONV1.num_of_w_bundle = 12;
  ALEXNET_IMGZ_CONV1.num_of_ia_bundle = ALEXNET_IMGZ_CONV1.ia_size*ALEXNET_IMGZ_CONV1.ia_size;
  ALEXNET_IMGZ_CONV1.num_of_oa_bundle = ALEXNET_IMGZ_CONV1.oa_size*ALEXNET_IMGZ_CONV1.oa_size;
  ALEXNET_IMGZ_CONV1.max_pool = false;
  ALEXNET_IMGZ_CONV1.quantize_M = 307;
  ALEXNET_IMGZ_CONV1.quantize_n = 17;

  ALEXNET_IMGZ_CONV2.relu = true;
  ALEXNET_IMGZ_CONV2.kernel_size = 3;
  ALEXNET_IMGZ_CONV2.stride = 1;
  ALEXNET_IMGZ_CONV2.ia_size = 63;
  ALEXNET_IMGZ_CONV2.ia_channel = 96;
  ALEXNET_IMGZ_CONV2.oa_size = 61;
  ALEXNET_IMGZ_CONV2.oa_channel = 128;
  ALEXNET_IMGZ_CONV2.w_bundle_set_index = 2;
  ALEXNET_IMGZ_CONV2.ia_bundle_set_index = 1;
  ALEXNET_IMGZ_CONV2.oa_bundle_set_index = 2;
  ALEXNET_IMGZ_CONV2.num_of_w_bundle = 429;
  ALEXNET_IMGZ_CONV2.num_of_ia_bundle = ALEXNET_IMGZ_CONV2.ia_size*ALEXNET_IMGZ_CONV2.ia_size;
  ALEXNET_IMGZ_CONV2.num_of_oa_bundle = ALEXNET_IMGZ_CONV2.oa_size*ALEXNET_IMGZ_CONV2.oa_size;
  ALEXNET_IMGZ_CONV2.max_pool = false;
  ALEXNET_IMGZ_CONV2.quantize_M = 393;
  ALEXNET_IMGZ_CONV2.quantize_n = 17;

  ALEXNET_IMGZ_MAXPOOL1.max_pool_kernel_size = 3;
  ALEXNET_IMGZ_MAXPOOL1.max_pool_stride = 2;
  ALEXNET_IMGZ_MAXPOOL1.ia_size = 61;
  ALEXNET_IMGZ_MAXPOOL1.ia_channel = 128;
  ALEXNET_IMGZ_MAXPOOL1.oa_size = 30;
  ALEXNET_IMGZ_MAXPOOL1.oa_channel = 128;
  ALEXNET_IMGZ_MAXPOOL1.ia_bundle_set_index = 2;
  ALEXNET_IMGZ_MAXPOOL1.oa_bundle_set_index = 3;
  ALEXNET_IMGZ_MAXPOOL1.num_of_ia_bundle = ALEXNET_IMGZ_MAXPOOL1.ia_size*ALEXNET_IMGZ_MAXPOOL1.ia_size;
  ALEXNET_IMGZ_MAXPOOL1.num_of_oa_bundle = ALEXNET_IMGZ_MAXPOOL1.oa_size*ALEXNET_IMGZ_MAXPOOL1.oa_size;

  ALEXNET_IMGZ_CONV3.relu = true;
  ALEXNET_IMGZ_CONV3.kernel_size = 3;
  ALEXNET_IMGZ_CONV3.stride = 1;
  ALEXNET_IMGZ_CONV3.ia_size = 30;
  ALEXNET_IMGZ_CONV3.ia_channel = 128;
  ALEXNET_IMGZ_CONV3.oa_size = 28;
  ALEXNET_IMGZ_CONV3.oa_channel = 160;
  ALEXNET_IMGZ_CONV3.w_bundle_set_index = 4;
  ALEXNET_IMGZ_CONV3.ia_bundle_set_index = 3;
  ALEXNET_IMGZ_CONV3.oa_bundle_set_index = 4;
  ALEXNET_IMGZ_CONV3.num_of_w_bundle = 708;
  ALEXNET_IMGZ_CONV3.num_of_ia_bundle = ALEXNET_IMGZ_CONV3.ia_size*ALEXNET_IMGZ_CONV3.ia_size;
  ALEXNET_IMGZ_CONV3.num_of_oa_bundle = ALEXNET_IMGZ_CONV3.oa_size*ALEXNET_IMGZ_CONV3.oa_size;
  ALEXNET_IMGZ_CONV3.max_pool = false;
  ALEXNET_IMGZ_CONV3.quantize_M = 318;
  ALEXNET_IMGZ_CONV3.quantize_n = 18;


  ALEXNET_IMGZ_MAXPOOL2.max_pool_kernel_size = 3;
  ALEXNET_IMGZ_MAXPOOL2.max_pool_stride = 2;
  ALEXNET_IMGZ_MAXPOOL2.ia_size = 28;
  ALEXNET_IMGZ_MAXPOOL2.ia_channel = 160;
  ALEXNET_IMGZ_MAXPOOL2.oa_size = 14;
  ALEXNET_IMGZ_MAXPOOL2.oa_channel = 160;
  ALEXNET_IMGZ_MAXPOOL2.ia_bundle_set_index = 4;
  ALEXNET_IMGZ_MAXPOOL2.oa_bundle_set_index = 5;
  ALEXNET_IMGZ_MAXPOOL2.num_of_ia_bundle = ALEXNET_IMGZ_MAXPOOL2.ia_size*ALEXNET_IMGZ_MAXPOOL2.ia_size;
  ALEXNET_IMGZ_MAXPOOL2.num_of_oa_bundle = ALEXNET_IMGZ_MAXPOOL2.oa_size*ALEXNET_IMGZ_MAXPOOL2.oa_size;


  ALEXNET_IMGZ_CONV4.relu = true;
  ALEXNET_IMGZ_CONV4.kernel_size = 3;
  ALEXNET_IMGZ_CONV4.stride = 1;
  ALEXNET_IMGZ_CONV4.ia_size = 14;
  ALEXNET_IMGZ_CONV4.ia_channel = 160;
  ALEXNET_IMGZ_CONV4.oa_size = 12;
  ALEXNET_IMGZ_CONV4.oa_channel = 192;
  ALEXNET_IMGZ_CONV4.w_bundle_set_index = 6;
  ALEXNET_IMGZ_CONV4.ia_bundle_set_index = 5;
  ALEXNET_IMGZ_CONV4.oa_bundle_set_index = 6;
  ALEXNET_IMGZ_CONV4.num_of_w_bundle = 1055;
  ALEXNET_IMGZ_CONV4.num_of_ia_bundle = ALEXNET_IMGZ_CONV4.ia_size*ALEXNET_IMGZ_CONV4.ia_size;
  ALEXNET_IMGZ_CONV4.num_of_oa_bundle = ALEXNET_IMGZ_CONV4.oa_size*ALEXNET_IMGZ_CONV4.oa_size;
  ALEXNET_IMGZ_CONV4.max_pool = false;
  ALEXNET_IMGZ_CONV4.quantize_M = 318;
  ALEXNET_IMGZ_CONV4.quantize_n = 18;


  ALEXNET_IMGZ_CONV5.relu = true;
  ALEXNET_IMGZ_CONV5.kernel_size = 3;
  ALEXNET_IMGZ_CONV5.stride = 1;
  ALEXNET_IMGZ_CONV5.ia_size = 12;
  ALEXNET_IMGZ_CONV5.ia_channel = 192;
  ALEXNET_IMGZ_CONV5.oa_size = 10;
  ALEXNET_IMGZ_CONV5.oa_channel = 224;
  ALEXNET_IMGZ_CONV5.w_bundle_set_index = 7;
  ALEXNET_IMGZ_CONV5.ia_bundle_set_index = 6;
  ALEXNET_IMGZ_CONV5.oa_bundle_set_index = 7;
  ALEXNET_IMGZ_CONV5.num_of_w_bundle = 1478; // FIXME:
  ALEXNET_IMGZ_CONV5.num_of_ia_bundle = ALEXNET_IMGZ_CONV5.ia_size*ALEXNET_IMGZ_CONV5.ia_size;
  ALEXNET_IMGZ_CONV5.num_of_oa_bundle = ALEXNET_IMGZ_CONV5.oa_size*ALEXNET_IMGZ_CONV5.oa_size;
  ALEXNET_IMGZ_CONV5.max_pool = false;
  ALEXNET_IMGZ_CONV5.quantize_M = 427;
  ALEXNET_IMGZ_CONV5.quantize_n = 18;


  ALEXNET_IMGZ_CONV6.relu = false;
  ALEXNET_IMGZ_CONV6.kernel_size = 3;
  ALEXNET_IMGZ_CONV6.stride = 1;
  ALEXNET_IMGZ_CONV6.ia_size = 10;
  ALEXNET_IMGZ_CONV6.ia_channel = 224;
  ALEXNET_IMGZ_CONV6.oa_size = 8;
  ALEXNET_IMGZ_CONV6.oa_channel = 256;
  ALEXNET_IMGZ_CONV6.w_bundle_set_index = 8;
  ALEXNET_IMGZ_CONV6.ia_bundle_set_index = 7;
  ALEXNET_IMGZ_CONV6.oa_bundle_set_index = 8;
  ALEXNET_IMGZ_CONV6.num_of_w_bundle = 2010; // FIXME:
  ALEXNET_IMGZ_CONV6.num_of_ia_bundle = ALEXNET_IMGZ_CONV6.ia_size*ALEXNET_IMGZ_CONV6.ia_size;
  ALEXNET_IMGZ_CONV6.num_of_oa_bundle = ALEXNET_IMGZ_CONV6.oa_size*ALEXNET_IMGZ_CONV6.oa_size;
  ALEXNET_IMGZ_CONV6.max_pool = false;
  ALEXNET_IMGZ_CONV6.quantize_M = 406;
  ALEXNET_IMGZ_CONV6.quantize_n = 20;


  C_Z_K.relu = false;
  C_Z_K.kernel_size = 3;
  C_Z_K.stride = 1;
  C_Z_K.ia_size = 8;
  C_Z_K.ia_channel = 256;
  C_Z_K.oa_size = 6;
  C_Z_K.oa_channel = 256;
  C_Z_K.w_bundle_set_index = 9;
  C_Z_K.ia_bundle_set_index = 8;
  C_Z_K.oa_bundle_set_index = 9;
  C_Z_K.num_of_ia_bundle = C_Z_K.ia_size*C_Z_K.ia_size;
  C_Z_K.num_of_oa_bundle = C_Z_K.oa_size*C_Z_K.oa_size;
  C_Z_K.num_of_w_bundle = 2228; // FIXME:
  C_Z_K.max_pool = false;
  C_Z_K.quantize_M = 381;
  C_Z_K.quantize_n = 17;


  R_Z_K.relu = false;
  R_Z_K.kernel_size = 3;
  R_Z_K.stride = 1;
  R_Z_K.ia_size = 8;
  R_Z_K.ia_channel = 256;
  R_Z_K.oa_size = 6;
  R_Z_K.oa_channel = 256;
  R_Z_K.w_bundle_set_index = 10;
  R_Z_K.ia_bundle_set_index = 8;
  R_Z_K.oa_bundle_set_index = 10;
  R_Z_K.num_of_ia_bundle = R_Z_K.ia_size*R_Z_K.ia_size;
  R_Z_K.num_of_oa_bundle = R_Z_K.oa_size*R_Z_K.oa_size;
  R_Z_K.num_of_w_bundle = 2222; // FIXME:
  R_Z_K.max_pool = false;
  R_Z_K.quantize_M = 282;
  R_Z_K.quantize_n = 17;

  // IMGX
  ALEXNET_IMGX_CONV1.relu = true;
  ALEXNET_IMGX_CONV1.kernel_size = 3;
  ALEXNET_IMGX_CONV1.stride = 2;
  ALEXNET_IMGX_CONV1.ia_size = 303;
  ALEXNET_IMGX_CONV1.ia_channel = 3;
  ALEXNET_IMGX_CONV1.oa_size = 151;
  ALEXNET_IMGX_CONV1.oa_channel = 96;
  ALEXNET_IMGX_CONV1.w_bundle_set_index = 12;
  ALEXNET_IMGX_CONV1.ia_bundle_set_index = 11;
  ALEXNET_IMGX_CONV1.oa_bundle_set_index = 12;
  ALEXNET_IMGX_CONV1.num_of_w_bundle = 12;
  ALEXNET_IMGX_CONV1.num_of_ia_bundle = ALEXNET_IMGX_CONV1.ia_size*ALEXNET_IMGX_CONV1.ia_size;
  ALEXNET_IMGX_CONV1.num_of_oa_bundle = ALEXNET_IMGX_CONV1.oa_size*ALEXNET_IMGX_CONV1.oa_size;
  ALEXNET_IMGX_CONV1.max_pool = false;
  ALEXNET_IMGX_CONV1.quantize_M = 307;
  ALEXNET_IMGX_CONV1.quantize_n = 17;

  ALEXNET_IMGX_CONV2.relu = true;
  ALEXNET_IMGX_CONV2.kernel_size = 3;
  ALEXNET_IMGX_CONV2.stride = 1;
  ALEXNET_IMGX_CONV2.ia_size = 151;
  ALEXNET_IMGX_CONV2.ia_channel = 96;
  ALEXNET_IMGX_CONV2.oa_size = 149;
  ALEXNET_IMGX_CONV2.oa_channel = 128;
  ALEXNET_IMGX_CONV2.w_bundle_set_index = 13;
  ALEXNET_IMGX_CONV2.ia_bundle_set_index = 12;
  ALEXNET_IMGX_CONV2.oa_bundle_set_index = 13;
  ALEXNET_IMGX_CONV2.num_of_w_bundle = 429;
  ALEXNET_IMGX_CONV2.num_of_ia_bundle = ALEXNET_IMGX_CONV2.ia_size*ALEXNET_IMGX_CONV2.ia_size;
  ALEXNET_IMGX_CONV2.num_of_oa_bundle = ALEXNET_IMGX_CONV2.oa_size*ALEXNET_IMGX_CONV2.oa_size;
  ALEXNET_IMGX_CONV2.max_pool = false;
  ALEXNET_IMGX_CONV2.quantize_M = 393;
  ALEXNET_IMGX_CONV2.quantize_n = 17;

  ALEXNET_IMGX_MAXPOOL1.max_pool_kernel_size = 3;
  ALEXNET_IMGX_MAXPOOL1.max_pool_stride = 2;
  ALEXNET_IMGX_MAXPOOL1.ia_size = 149;
  ALEXNET_IMGX_MAXPOOL1.ia_channel = 128;
  ALEXNET_IMGX_MAXPOOL1.oa_size = 74;
  ALEXNET_IMGX_MAXPOOL1.oa_channel = 128;
  ALEXNET_IMGX_MAXPOOL1.ia_bundle_set_index = 13;
  ALEXNET_IMGX_MAXPOOL1.oa_bundle_set_index = 14;
  ALEXNET_IMGX_MAXPOOL1.num_of_ia_bundle = ALEXNET_IMGX_MAXPOOL1.ia_size*ALEXNET_IMGX_MAXPOOL1.ia_size;
  ALEXNET_IMGX_MAXPOOL1.num_of_oa_bundle = ALEXNET_IMGX_MAXPOOL1.oa_size*ALEXNET_IMGX_MAXPOOL1.oa_size;

  ALEXNET_IMGX_CONV3.relu = true;
  ALEXNET_IMGX_CONV3.kernel_size = 3;
  ALEXNET_IMGX_CONV3.stride = 1;
  ALEXNET_IMGX_CONV3.ia_size = 74;
  ALEXNET_IMGX_CONV3.ia_channel = 128;
  ALEXNET_IMGX_CONV3.oa_size = 72;
  ALEXNET_IMGX_CONV3.oa_channel = 160;
  ALEXNET_IMGX_CONV3.w_bundle_set_index = 15;
  ALEXNET_IMGX_CONV3.ia_bundle_set_index = 14;
  ALEXNET_IMGX_CONV3.oa_bundle_set_index = 15;
  ALEXNET_IMGX_CONV3.num_of_w_bundle = 708;
  ALEXNET_IMGX_CONV3.num_of_ia_bundle = ALEXNET_IMGX_CONV3.ia_size*ALEXNET_IMGX_CONV3.ia_size;
  ALEXNET_IMGX_CONV3.num_of_oa_bundle = ALEXNET_IMGX_CONV3.oa_size*ALEXNET_IMGX_CONV3.oa_size;
  ALEXNET_IMGX_CONV3.max_pool = false;
  ALEXNET_IMGX_CONV3.quantize_M = 318;
  ALEXNET_IMGX_CONV3.quantize_n = 18;


  ALEXNET_IMGX_MAXPOOL2.max_pool_kernel_size = 3;
  ALEXNET_IMGX_MAXPOOL2.max_pool_stride = 2;
  ALEXNET_IMGX_MAXPOOL2.ia_size = 72;
  ALEXNET_IMGX_MAXPOOL2.ia_channel = 160;
  ALEXNET_IMGX_MAXPOOL2.oa_size = 36;
  ALEXNET_IMGX_MAXPOOL2.oa_channel = 160;
  ALEXNET_IMGX_MAXPOOL2.ia_bundle_set_index = 15;
  ALEXNET_IMGX_MAXPOOL2.oa_bundle_set_index = 16;
  ALEXNET_IMGX_MAXPOOL2.num_of_ia_bundle = ALEXNET_IMGX_MAXPOOL2.ia_size*ALEXNET_IMGX_MAXPOOL2.ia_size;
  ALEXNET_IMGX_MAXPOOL2.num_of_oa_bundle = ALEXNET_IMGX_MAXPOOL2.oa_size*ALEXNET_IMGX_MAXPOOL2.oa_size;


  ALEXNET_IMGX_CONV4.relu = true;
  ALEXNET_IMGX_CONV4.kernel_size = 3;
  ALEXNET_IMGX_CONV4.stride = 1;
  ALEXNET_IMGX_CONV4.ia_size = 36;
  ALEXNET_IMGX_CONV4.ia_channel = 160;
  ALEXNET_IMGX_CONV4.oa_size = 34;
  ALEXNET_IMGX_CONV4.oa_channel = 192;
  ALEXNET_IMGX_CONV4.w_bundle_set_index = 17;
  ALEXNET_IMGX_CONV4.ia_bundle_set_index = 16;
  ALEXNET_IMGX_CONV4.oa_bundle_set_index = 17;
  ALEXNET_IMGX_CONV4.num_of_w_bundle = 1055;
  ALEXNET_IMGX_CONV4.num_of_ia_bundle = ALEXNET_IMGX_CONV4.ia_size*ALEXNET_IMGX_CONV4.ia_size;
  ALEXNET_IMGX_CONV4.num_of_oa_bundle = ALEXNET_IMGX_CONV4.oa_size*ALEXNET_IMGX_CONV4.oa_size;
  ALEXNET_IMGX_CONV4.max_pool = false;
  ALEXNET_IMGX_CONV4.quantize_M = 318;
  ALEXNET_IMGX_CONV4.quantize_n = 18;


  ALEXNET_IMGX_CONV5.relu = true;
  ALEXNET_IMGX_CONV5.kernel_size = 3;
  ALEXNET_IMGX_CONV5.stride = 1;
  ALEXNET_IMGX_CONV5.ia_size = 34;
  ALEXNET_IMGX_CONV5.ia_channel = 192;
  ALEXNET_IMGX_CONV5.oa_size = 32;
  ALEXNET_IMGX_CONV5.oa_channel = 224;
  ALEXNET_IMGX_CONV5.w_bundle_set_index = 18;
  ALEXNET_IMGX_CONV5.ia_bundle_set_index = 17;
  ALEXNET_IMGX_CONV5.oa_bundle_set_index = 18;
  ALEXNET_IMGX_CONV5.num_of_w_bundle = 1478; // FIXME:
  ALEXNET_IMGX_CONV5.num_of_ia_bundle = ALEXNET_IMGX_CONV5.ia_size*ALEXNET_IMGX_CONV5.ia_size;
  ALEXNET_IMGX_CONV5.num_of_oa_bundle = ALEXNET_IMGX_CONV5.oa_size*ALEXNET_IMGX_CONV5.oa_size;
  ALEXNET_IMGX_CONV5.max_pool = false;
  ALEXNET_IMGX_CONV5.quantize_M = 427;
  ALEXNET_IMGX_CONV5.quantize_n = 18;


  ALEXNET_IMGX_CONV6.relu = false;
  ALEXNET_IMGX_CONV6.kernel_size = 3;
  ALEXNET_IMGX_CONV6.stride = 1;
  ALEXNET_IMGX_CONV6.ia_size = 32;
  ALEXNET_IMGX_CONV6.ia_channel = 224;
  ALEXNET_IMGX_CONV6.oa_size = 30;
  ALEXNET_IMGX_CONV6.oa_channel = 256;
  ALEXNET_IMGX_CONV6.w_bundle_set_index = 19;
  ALEXNET_IMGX_CONV6.ia_bundle_set_index = 18;
  ALEXNET_IMGX_CONV6.oa_bundle_set_index = 19;
  ALEXNET_IMGX_CONV6.num_of_w_bundle = 2010; // FIXME:
  ALEXNET_IMGX_CONV6.num_of_ia_bundle = ALEXNET_IMGX_CONV6.ia_size*ALEXNET_IMGX_CONV6.ia_size;
  ALEXNET_IMGX_CONV6.num_of_oa_bundle = ALEXNET_IMGX_CONV6.oa_size*ALEXNET_IMGX_CONV6.oa_size;
  ALEXNET_IMGX_CONV6.max_pool = false;
  ALEXNET_IMGX_CONV6.quantize_M = 406;
  ALEXNET_IMGX_CONV6.quantize_n = 20;


  C_X.relu = false;
  C_X.kernel_size = 3;
  C_X.stride = 1;
  C_X.ia_size = 30;
  C_X.ia_channel = 256;
  C_X.oa_size = 28;
  C_X.oa_channel = 256;
  C_X.w_bundle_set_index = 20;
  C_X.ia_bundle_set_index = 19;
  C_X.oa_bundle_set_index = 20;
  C_X.num_of_ia_bundle = C_X.ia_size*C_X.ia_size;
  C_X.num_of_oa_bundle = C_X.oa_size*C_X.oa_size;
  C_X.num_of_w_bundle = 2219; // FIXME:
  C_X.max_pool = false;
  C_X.quantize_M = 383;
  C_X.quantize_n = 18;


  R_X.relu = false;
  R_X.kernel_size = 3;
  R_X.stride = 1;
  R_X.ia_size = 30;
  R_X.ia_channel = 256;
  R_X.oa_size = 28;
  R_X.oa_channel = 256;
  R_X.w_bundle_set_index = 21;
  R_X.ia_bundle_set_index = 19;
  R_X.oa_bundle_set_index = 21;
  R_X.num_of_ia_bundle = R_X.ia_size*R_X.ia_size;
  R_X.num_of_oa_bundle = R_X.oa_size*R_X.oa_size;
  R_X.num_of_w_bundle = 2204; // FIXME:
  R_X.max_pool = false;
  R_X.quantize_M = 354;
  R_X.quantize_n = 18;

  
  C_Z.relu = false;
  C_Z.kernel_size = 6;
  C_Z.stride = 1;
  C_Z.ia_size = 28;
  C_Z.ia_channel = 256;
  C_Z.oa_size = 23;
  C_Z.oa_channel = 256;
  C_Z.w_bundle_set_index = 40;
  C_Z.ia_bundle_set_index = 20;
  C_Z.oa_bundle_set_index = 22;
  C_Z.num_of_ia_bundle = C_Z.ia_size*C_Z.ia_size;
  C_Z.num_of_oa_bundle = C_Z.oa_size*C_Z.oa_size;
  C_Z.num_of_w_bundle = 0; // FIXME:
  C_Z.max_pool = false;
  C_Z.quantize_M = 419;
  C_Z.quantize_n = 18;


  R_Z.relu = false;
  R_Z.kernel_size = 6;
  R_Z.stride = 1;
  R_Z.ia_size = 28;
  R_Z.ia_channel = 256;
  R_Z.oa_size = 23;
  R_Z.oa_channel = 256;
  R_Z.w_bundle_set_index = 41;
  R_Z.ia_bundle_set_index = 21;
  R_Z.oa_bundle_set_index = 28;
  R_Z.num_of_ia_bundle = R_Z.ia_size*R_Z.ia_size;
  R_Z.num_of_oa_bundle = R_Z.oa_size*R_Z.oa_size;
  R_Z.num_of_w_bundle = 0; // FIXME:
  R_Z.max_pool = false;
  R_Z.quantize_M = 500;
  R_Z.quantize_n = 18;



  C_CONV1.relu = true;
  C_CONV1.kernel_size = 3;
  C_CONV1.stride = 1;
  C_CONV1.ia_size = 23;
  C_CONV1.ia_channel = 256;
  C_CONV1.oa_size = 21;
  C_CONV1.oa_channel = 256;
  C_CONV1.w_bundle_set_index = 23;
  C_CONV1.ia_bundle_set_index = 22;
  C_CONV1.oa_bundle_set_index = 23;
  C_CONV1.num_of_ia_bundle = C_CONV1.ia_size*C_CONV1.ia_size;
  C_CONV1.num_of_oa_bundle = C_CONV1.oa_size*C_CONV1.oa_size;
  C_CONV1.num_of_w_bundle = 2204; // FIXME:
  C_CONV1.max_pool = false;
  C_CONV1.quantize_M = 369;
  C_CONV1.quantize_n = 18;

  C_CONV2.relu = true;
  C_CONV2.kernel_size = 3;
  C_CONV2.stride = 1;
  C_CONV2.ia_size = 21;
  C_CONV2.ia_channel = 256;
  C_CONV2.oa_size = 19;
  C_CONV2.oa_channel = 256;
  C_CONV2.w_bundle_set_index = 24;
  C_CONV2.ia_bundle_set_index = 23;
  C_CONV2.oa_bundle_set_index = 24;
  C_CONV2.num_of_ia_bundle = C_CONV2.ia_size*C_CONV2.ia_size;
  C_CONV2.num_of_oa_bundle = C_CONV2.oa_size*C_CONV2.oa_size;
  C_CONV2.num_of_w_bundle = 2085; // FIXME:
  C_CONV2.max_pool = false;
  C_CONV2.quantize_M = 296;
  C_CONV2.quantize_n = 16;

  C_CONV3.relu = true;
  C_CONV3.kernel_size = 3;
  C_CONV3.stride = 1;
  C_CONV3.ia_size = 19;
  C_CONV3.ia_channel = 256;
  C_CONV3.oa_size = 17;
  C_CONV3.oa_channel = 256;
  C_CONV3.w_bundle_set_index = 25;
  C_CONV3.ia_bundle_set_index = 24;
  C_CONV3.oa_bundle_set_index = 25;
  C_CONV3.num_of_ia_bundle = C_CONV3.ia_size*C_CONV3.ia_size;
  C_CONV3.num_of_oa_bundle = C_CONV3.oa_size*C_CONV3.oa_size;
  C_CONV3.num_of_w_bundle = 2193; // FIXME:
  C_CONV3.max_pool = false;
  C_CONV3.quantize_M = 366;
  C_CONV3.quantize_n = 17;

  R_CONV1.relu = true;
  R_CONV1.kernel_size = 3;
  R_CONV1.stride = 1;
  R_CONV1.ia_size = 23;
  R_CONV1.ia_channel = 256;
  R_CONV1.oa_size = 21;
  R_CONV1.oa_channel = 256;
  R_CONV1.w_bundle_set_index = 29;
  R_CONV1.ia_bundle_set_index = 28;
  R_CONV1.oa_bundle_set_index = 29;
  R_CONV1.num_of_ia_bundle = R_CONV1.ia_size*R_CONV1.ia_size;
  R_CONV1.num_of_oa_bundle = R_CONV1.oa_size*R_CONV1.oa_size;
  R_CONV1.num_of_w_bundle = 2072; // FIXME:
  R_CONV1.max_pool = false;
  R_CONV1.quantize_M = 443;
  R_CONV1.quantize_n = 18;

  R_CONV2.relu = true;
  R_CONV2.kernel_size = 3;
  R_CONV2.stride = 1;
  R_CONV2.ia_size = 21;
  R_CONV2.ia_channel = 256;
  R_CONV2.oa_size = 19;
  R_CONV2.oa_channel = 256;
  R_CONV2.w_bundle_set_index = 30;
  R_CONV2.ia_bundle_set_index = 29;
  R_CONV2.oa_bundle_set_index = 30;
  R_CONV2.num_of_ia_bundle = R_CONV2.ia_size*R_CONV2.ia_size;
  R_CONV2.num_of_oa_bundle = R_CONV2.oa_size*R_CONV2.oa_size;
  R_CONV2.num_of_w_bundle = 1077; // FIXME:
  R_CONV2.max_pool = false;
  R_CONV2.quantize_M = 445;
  R_CONV2.quantize_n = 15;

  R_CONV3.relu = true;
  R_CONV3.kernel_size = 3;
  R_CONV3.stride = 1;
  R_CONV3.ia_size = 19;
  R_CONV3.ia_channel = 256;
  R_CONV3.oa_size = 17;
  R_CONV3.oa_channel = 256;
  R_CONV3.w_bundle_set_index = 31;
  R_CONV3.ia_bundle_set_index = 30;
  R_CONV3.oa_bundle_set_index = 31;
  R_CONV3.num_of_ia_bundle = R_CONV3.ia_size*R_CONV3.ia_size;
  R_CONV3.num_of_oa_bundle = R_CONV3.oa_size*R_CONV3.oa_size;
  R_CONV3.num_of_w_bundle = 1852; // FIXME:
  R_CONV3.max_pool = false;
  R_CONV3.quantize_M = 310;
  R_CONV3.quantize_n = 15;



  C_CLS_CONV.relu = false;
  C_CLS_CONV.kernel_size = 1;
  C_CLS_CONV.stride = 1;
  C_CLS_CONV.ia_size = 17;
  C_CLS_CONV.ia_channel = 256;
  C_CLS_CONV.oa_size = 17;
  C_CLS_CONV.oa_channel = 1;
  C_CLS_CONV.w_bundle_set_index = 26;
  C_CLS_CONV.ia_bundle_set_index = 25;
  C_CLS_CONV.oa_bundle_set_index = 26;
  C_CLS_CONV.num_of_ia_bundle = C_CLS_CONV.ia_size*C_CLS_CONV.ia_size;
  C_CLS_CONV.num_of_oa_bundle = C_CLS_CONV.oa_size*C_CLS_CONV.oa_size;
  C_CLS_CONV.num_of_w_bundle = 2; // FIXME:
  C_CLS_CONV.max_pool = false;
  C_CLS_CONV.quantize_M = 364;
  C_CLS_CONV.quantize_n = 17;

  C_CTR_CONV.relu = false;
  C_CTR_CONV.kernel_size = 1;
  C_CTR_CONV.stride = 1;
  C_CTR_CONV.ia_size = 17;
  C_CTR_CONV.ia_channel = 256;
  C_CTR_CONV.oa_size = 17;
  C_CTR_CONV.oa_channel = 1;
  C_CTR_CONV.w_bundle_set_index = 27;
  C_CTR_CONV.ia_bundle_set_index = 25;
  C_CTR_CONV.oa_bundle_set_index = 27;
  C_CTR_CONV.num_of_ia_bundle = C_CTR_CONV.ia_size*C_CTR_CONV.ia_size;
  C_CTR_CONV.num_of_oa_bundle = C_CTR_CONV.oa_size*C_CTR_CONV.oa_size;
  C_CTR_CONV.num_of_w_bundle = 1; // FIXME:
  C_CTR_CONV.max_pool = false;
  C_CTR_CONV.quantize_M = 271;
  C_CTR_CONV.quantize_n = 16;

  R_REG_CONV.relu = false;
  R_REG_CONV.kernel_size = 1;
  R_REG_CONV.stride = 1;
  R_REG_CONV.ia_size = 17;
  R_REG_CONV.ia_channel = 256;
  R_REG_CONV.oa_size = 17;
  R_REG_CONV.oa_channel = 4;
  R_REG_CONV.w_bundle_set_index = 32;
  R_REG_CONV.ia_bundle_set_index = 31;
  R_REG_CONV.oa_bundle_set_index = 32;
  R_REG_CONV.num_of_ia_bundle = R_REG_CONV.ia_size*R_REG_CONV.ia_size;
  R_REG_CONV.num_of_oa_bundle = R_REG_CONV.oa_size*R_REG_CONV.oa_size;
  R_REG_CONV.num_of_w_bundle = 4; // FIXME:
  R_REG_CONV.max_pool = false;
  R_REG_CONV.quantize_M = 504;
  R_REG_CONV.quantize_n = 17;


  // TEST_CONV.relu = false;
  // TEST_CONV.kernel_size = 3;
  // TEST_CONV.stride = 1;
  // TEST_CONV.ia_size = 8;
  // TEST_CONV.ia_channel = 3;
  // TEST_CONV.oa_size = 6;
  // TEST_CONV.oa_channel = 16;
  // TEST_CONV.w_bundle_set_index = 100;
  // TEST_CONV.ia_bundle_set_index = 100;
  // TEST_CONV.oa_bundle_set_index = 101;
  // TEST_CONV.num_of_w_bundle = 3; // FIXME:
  // TEST_CONV.num_of_ia_bundle = TEST_CONV.ia_size*TEST_CONV.ia_size;
  // TEST_CONV.num_of_oa_bundle = TEST_CONV.oa_size*TEST_CONV.oa_size;
  // TEST_CONV.max_pool = false;
  // TEST_CONV.quantize_M = 1;
  // TEST_CONV.quantize_n = 0;

  TEST_CONV_S2.relu = false;
  TEST_CONV_S2.kernel_size = 3;
  TEST_CONV_S2.stride = 2;
  TEST_CONV_S2.ia_size = 127;
  TEST_CONV_S2.ia_channel = 3;
  TEST_CONV_S2.oa_size = 63;
  TEST_CONV_S2.oa_channel = 96;
  TEST_CONV_S2.w_bundle_set_index = 100;
  TEST_CONV_S2.ia_bundle_set_index = 100;
  TEST_CONV_S2.oa_bundle_set_index = 101;
  TEST_CONV_S2.num_of_w_bundle = 12; // FIXME:
  TEST_CONV_S2.num_of_ia_bundle = TEST_CONV_S2.ia_size*TEST_CONV_S2.ia_size;
  TEST_CONV_S2.num_of_oa_bundle = TEST_CONV_S2.oa_size*TEST_CONV_S2.oa_size;
  TEST_CONV_S2.max_pool = false;
  TEST_CONV_S2.quantize_M = 19;
  TEST_CONV_S2.quantize_n = 13;
}