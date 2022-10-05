#include "../inc/memory_controller.h"
#include "../inc/address_config_v1.h"
#include "../inc/specs.h"
#include <vector>
#include <tuple>


//Dram
std::tuple<int, int, int, int> DramAddressGenerator::AddressStartAndEndOfInputActivationBundleSet(int intput_ft_map_idx){
  switch (intput_ft_map_idx){
    case (0): //img_z.
      return std::tie(kimg_z_addr_depth_start,kimg_z_addr_depth_end,kimg_z_addr_data_start,kimg_z_addr_end); // start, end
    case (11): //img_x
      return std::tie(kimg_x_addr_depth_start,kimg_x_addr_depth_end,kimg_x_addr_data_start,kimg_x_addr_end); // start, end
    case (1): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_1, koa_ft_map_addr_depth_end_1,koa_ft_map_addr_data_end_1); 
    case (2): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_2, koa_ft_map_addr_depth_end_2,koa_ft_map_addr_data_end_2); 
    case (3): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_3, koa_ft_map_addr_depth_end_3,koa_ft_map_addr_data_end_3); 
    case (4): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_4, koa_ft_map_addr_depth_end_4,koa_ft_map_addr_data_end_4); 
    case (5): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_5, koa_ft_map_addr_depth_end_5,koa_ft_map_addr_data_end_5); 
    case (6): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_6, koa_ft_map_addr_depth_end_6,koa_ft_map_addr_data_end_6); 
    case (7): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_7, koa_ft_map_addr_depth_end_7,koa_ft_map_addr_data_end_7); 
    case (8): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_8, koa_ft_map_addr_depth_end_8,koa_ft_map_addr_data_end_8); 
    // case (9): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_9, koa_ft_map_addr_depth_end_9,koa_ft_map_addr_data_end_9); 
    // case (10): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_10, koa_ft_map_addr_depth_end_10,koa_ft_map_addr_data_end_10); 
    case (12): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_12, koa_ft_map_addr_depth_end_12,koa_ft_map_addr_data_end_12); 
    case (13): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_13, koa_ft_map_addr_depth_end_13,koa_ft_map_addr_data_end_13); 
    case (14): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_14, koa_ft_map_addr_depth_end_14,koa_ft_map_addr_data_end_14); 
    case (15): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_15, koa_ft_map_addr_depth_end_15,koa_ft_map_addr_data_end_15); 
    case (16): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_16, koa_ft_map_addr_depth_end_16,koa_ft_map_addr_data_end_15); 
    case (17): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_17, koa_ft_map_addr_depth_end_17,koa_ft_map_addr_data_end_17); 
    case (18): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_18, koa_ft_map_addr_depth_end_18,koa_ft_map_addr_data_end_18); 
    case (19): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_19, koa_ft_map_addr_depth_end_19,koa_ft_map_addr_data_end_19); 
    // c_x, r_x
    case (20): return std::tie(koa_ft_map_c_x_addr_depth_start,koa_ft_map_c_x_addr_depth_end, koa_ft_map_c_x_addr_data_start,koa_ft_map_c_x_addr_end); 
    case (21): return std::tie(koa_ft_map_r_x_addr_depth_start,koa_ft_map_r_x_addr_depth_end, koa_ft_map_r_x_addr_data_start,koa_ft_map_r_x_addr_end); 
    // case (20): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_20, koa_ft_map_addr_depth_end_20,koa_ft_map_addr_data_end_20); 
    // case (21): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_21, koa_ft_map_addr_depth_end_21,koa_ft_map_addr_data_end_21); 
    case (22): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_22, koa_ft_map_addr_depth_end_22,koa_ft_map_addr_data_end_22); 
    case (23): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_23, koa_ft_map_addr_depth_end_23,koa_ft_map_addr_data_end_23); 
    case (24): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_24, koa_ft_map_addr_depth_end_24,koa_ft_map_addr_data_end_24); 
    case (25): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_25, koa_ft_map_addr_depth_end_25,koa_ft_map_addr_data_end_25); 
    case (26): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_26, koa_ft_map_addr_depth_end_26,koa_ft_map_addr_data_end_26); 
    case (27): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_27, koa_ft_map_addr_depth_end_27,koa_ft_map_addr_data_end_27); 
    case (28): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_28, koa_ft_map_addr_depth_end_28,koa_ft_map_addr_data_end_28); 
    case (29): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_29, koa_ft_map_addr_depth_end_29,koa_ft_map_addr_data_end_29); 
    case (30): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_30, koa_ft_map_addr_depth_end_30,koa_ft_map_addr_data_end_30); 
    case (31): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_31, koa_ft_map_addr_depth_end_31,koa_ft_map_addr_data_end_31); 
    case (32): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_32, koa_ft_map_addr_depth_end_32,koa_ft_map_addr_data_end_32); 
    
    case (100): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_100, koa_ft_map_addr_depth_end_100,koa_ft_map_addr_data_end_100); 
    case (101): return std::tie(koa_ft_map_addr_depth_start,koa_ft_map_addr_depth_end_101, koa_ft_map_addr_depth_end_101,koa_ft_map_addr_data_end_101); 
    
    
    default: return std::tie(kimg_z_addr_depth_start,kimg_z_addr_depth_end,kimg_z_addr_data_start,kimg_z_addr_end);
  }
}

std::tuple<int,int> DramAddressGenerator::AddressStartAndEndOfWeightBundleSet(int weight_idx){
  switch (weight_idx){
    case (1): case(12): //conv1.
      return std::tie(kconv1_addr_s_start, kconv1_addr_end);
    case (2): case(13): //conv2
      return std::tie(kconv2_addr_s_start, kconv2_addr_end);
    case (4): case(15): //conv3
      return std::tie(kconv3_addr_s_start, kconv3_addr_end);
    case (6): case(17): //conv4
      return std::tie(kconv4_addr_s_start, kconv4_addr_end);
    case (7): case(18): //conv5
      return std::tie(kconv5_addr_s_start, kconv5_addr_end);
    case (8): case(19): //conv6
      return std::tie(kconv6_addr_s_start, kconv6_addr_end);
    case (9): //c_z_k
      return std::tie(kc_z_k_addr_s_start, kc_z_k_addr_end);
    case (10): //r_z_k
      return std::tie(kr_z_k_addr_s_start, kr_z_k_addr_end);
    case (20): //c_x
      return std::tie(kc_x_addr_s_start, kc_x_addr_end);
    case (21): //r_x
      return std::tie(kr_x_addr_s_start, kr_x_addr_end);
    case (23): //cls_conv1
      return std::tie(kcls_conv1_addr_s_start, kcls_conv1_addr_end);
    case (24): //cls_conv2
      return std::tie(kcls_conv2_addr_s_start, kcls_conv2_addr_end);
    case (25): //cls_conv3
      return std::tie(kcls_conv3_addr_s_start, kcls_conv3_addr_end);
    case (26): //cls_score
      return std::tie(kcls_score_addr_s_start, kcls_score_addr_end);
    case (27): //ctr_score
      return std::tie(kctr_score_addr_s_start, kctr_score_addr_end);
    case (29): //bbox_conv1
      return std::tie(kbbox_conv1_addr_s_start, kbbox_conv1_addr_end);
    case (30): //bbox_conv2
      return std::tie(kbbox_conv2_addr_s_start, kbbox_conv2_addr_end);
    case (31): //bbox_conv3
      return std::tie(kbbox_conv3_addr_s_start, kbbox_conv3_addr_end);
    case (32): //offset_score
      return std::tie(koffset_score_addr_s_start, koffset_score_addr_end);
      
    case (40): //c_z
      return std::tie(kc_z_addr_depth_start, kc_z_addr_end);
    case (41): //r_z
      return std::tie(kr_z_addr_depth_start, kr_z_addr_end);

    case (100): //test_conv
      return std::tie(ktest_conv_addr_s_start, ktest_conv_addr_end);

    default:
      int default0=0;
      return std::tie(default0, default0);
  }
}

int DramAddressGenerator::AddressDepthOfInputActivationBundle(int bundle_idx){
  return this->addr_depth_start + bundle_idx/kDramAccessBytes;
}

int DramAddressGenerator::AddressDataOfInputActivationBundle(int bundle_idx){
  return this->addr_data_start + bundle_idx*this->addr_interval_of_data;
}

int DramAddressGenerator::AddressDataEndOfInputActivationBundle(int bundle_idx){
  return this->addr_data_start + (bundle_idx+1)*this->addr_interval_of_data;
}

//For z kernel
int DramAddressGenerator::AddressZKernelDepthOfInputActivationBundle(int s){
  return this->addr_depth_start + s;
}

int DramAddressGenerator::AddressZKernelDataOfInputActivationBundle(int s){
  return this->addr_data_start + (s*6*specs::kWBundleMaxSize*2)/kDramAccessBytes;
}

int DramAddressGenerator::AddressZKernelDataEndOfInputActivationBundle(int s){
  return this->addr_data_start + (s+1)*6*specs::kWBundleMaxSize*2/kDramAccessBytes;
}


//For result
int DramAddressGenerator::AddressResultDataOfInputActivationBundle(int bundle_idx){
  return this->addr_data_start + bundle_idx/2;
}

void DramAddressGenerator::CalibrateWriteAddress(int output_ft_map_idx){
  this->addr_depth_start      = koa_ft_map_addr_depth_start;
  this->addr_data_start       = koa_ft_map_addr_data_start;
  this->addr_interval_of_data = koa_ft_map_addr_interval_of_data;
  switch (output_ft_map_idx){
    // case(0): img_z
    case(1): this->addr_data_start=koa_ft_map_addr_depth_end_1; this->addr_interval_of_data = koa_ft_map_conv1_interval_of_data; break;
    case(2): this->addr_data_start=koa_ft_map_addr_depth_end_2; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(3): this->addr_data_start=koa_ft_map_addr_depth_end_3; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(4): this->addr_data_start=koa_ft_map_addr_depth_end_4; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(5): this->addr_data_start=koa_ft_map_addr_depth_end_5; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(6): this->addr_data_start=koa_ft_map_addr_depth_end_6; this->addr_interval_of_data = koa_ft_map_conv4_interval_of_data; break;
    case(7): this->addr_data_start=koa_ft_map_addr_depth_end_7; this->addr_interval_of_data = koa_ft_map_conv5_interval_of_data; break;
    case(8): this->addr_data_start=koa_ft_map_addr_depth_end_8; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    // case(11):img_x
    case(12): this->addr_data_start=koa_ft_map_addr_depth_end_12; this->addr_interval_of_data = koa_ft_map_conv1_interval_of_data; break;
    case(13): this->addr_data_start=koa_ft_map_addr_depth_end_13; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(14): this->addr_data_start=koa_ft_map_addr_depth_end_14; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(15): this->addr_data_start=koa_ft_map_addr_depth_end_15; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(16): this->addr_data_start=koa_ft_map_addr_depth_end_16; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(17): this->addr_data_start=koa_ft_map_addr_depth_end_17; this->addr_interval_of_data = koa_ft_map_conv4_interval_of_data; break;
    case(18): this->addr_data_start=koa_ft_map_addr_depth_end_18; this->addr_interval_of_data = koa_ft_map_conv5_interval_of_data; break;
    case(19): this->addr_data_start=koa_ft_map_addr_depth_end_19; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    
    case(20): this->addr_depth_start = koa_ft_map_c_x_addr_depth_start;
              this->addr_data_start = koa_ft_map_c_x_addr_depth_end;
              this->addr_interval_of_data = koa_ft_map_c_x_addr_interval_of_data; break;
    case(21): this->addr_depth_start = koa_ft_map_r_x_addr_depth_start;
              this->addr_data_start = koa_ft_map_r_x_addr_depth_end;
              this->addr_interval_of_data = koa_ft_map_r_x_addr_interval_of_data; break;

    
    //cls
    case(22): this->addr_data_start=koa_ft_map_addr_depth_end_22; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    case(23): this->addr_data_start=koa_ft_map_addr_depth_end_23; this->addr_interval_of_data = koa_ft_map_cls_conv1_interval_of_data; break;
    case(24): this->addr_data_start=koa_ft_map_addr_depth_end_24; this->addr_interval_of_data = koa_ft_map_cls_conv2_interval_of_data; break;
    case(25): this->addr_data_start=koa_ft_map_addr_depth_end_25; this->addr_interval_of_data = koa_ft_map_cls_conv3_interval_of_data; break;
    case(26): this->addr_data_start=kresult_cls_score_start; break;
    //ctr
    case(27): this->addr_data_start=kresult_ctr_score_start; break;
    //bbox
    case(28): this->addr_data_start=koa_ft_map_addr_depth_end_28; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    case(29): this->addr_data_start=koa_ft_map_addr_depth_end_29; this->addr_interval_of_data = koa_ft_map_bbox_conv1_interval_of_data; break;
    case(30): this->addr_data_start=koa_ft_map_addr_depth_end_30; this->addr_interval_of_data = koa_ft_map_bbox_conv2_interval_of_data; break;
    case(31): this->addr_data_start=koa_ft_map_addr_depth_end_31; this->addr_interval_of_data = koa_ft_map_bbox_conv3_interval_of_data; break;
    case(32): this->addr_data_start=kresult_offset_score_start; break;
    
    case(9): //c_z
        this->addr_depth_start = kc_z_addr_depth_start;
        this->addr_data_start = kc_z_addr_data_start;
        this->addr_interval_of_data = kc_z_addr_interval_of_data;
        break;
    case(10): //r_z
        this->addr_depth_start = kr_z_addr_depth_start;
        this->addr_data_start = kr_z_addr_data_start;
        this->addr_interval_of_data = kr_z_addr_interval_of_data;
        break;

    case(101): this->addr_data_start=koa_ft_map_addr_depth_end_101; this->addr_interval_of_data = koa_ft_map_test_conv_output_interval_of_data; break;
  }
}






//IaSram
int IaSramAddressGenerator::AddressDepthOfInputActivationBundle(int bundle_idx){
  return this->addr_depth_start + bundle_idx/kSramAccessBytes;
}

int IaSramAddressGenerator::AddressDataOfInputActivationBundle(int bundle_idx){
  return this->addr_data_start + bundle_idx * this->addr_interval_of_data;
}

int IaSramAddressGenerator::AddressDataEndOfInputActivationBundle(int bundle_idx){
  return this->addr_data_start + (bundle_idx+1) * this->addr_interval_of_data;
}

void IaSramAddressGenerator::Calibrate(int input_ft_map_idx){
  this->addr_depth_start      = 0;
  this->addr_data_start       = koa_ft_map_addr_data_start-koa_ft_map_addr_depth_start;
  
  switch (input_ft_map_idx){
    case(0): //img_z
      this->addr_data_start       = kimg_z_addr_data_start-kimg_z_addr_depth_start;
      this->addr_interval_of_data = kimg_z_addr_interval_of_data;
      break;
    case(11): //img_x
      this->addr_data_start       = kimg_x_addr_data_start-kimg_x_addr_depth_start;
      this->addr_interval_of_data = kimg_x_addr_interval_of_data;
      break;
    case(1): this->addr_data_start=koa_ft_map_addr_depth_end_1-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv1_interval_of_data; break;
    case(2): this->addr_data_start=koa_ft_map_addr_depth_end_2-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(3): this->addr_data_start=koa_ft_map_addr_depth_end_3-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(4): this->addr_data_start=koa_ft_map_addr_depth_end_4-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(5): this->addr_data_start=koa_ft_map_addr_depth_end_5-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(6): this->addr_data_start=koa_ft_map_addr_depth_end_6-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv4_interval_of_data; break;
    case(7): this->addr_data_start=koa_ft_map_addr_depth_end_7-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv5_interval_of_data; break;
    case(8): this->addr_data_start=koa_ft_map_addr_depth_end_8-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    // case(11):img_x
    case(12): this->addr_data_start=koa_ft_map_addr_depth_end_12-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv1_interval_of_data; break;
    case(13): this->addr_data_start=koa_ft_map_addr_depth_end_13-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(14): this->addr_data_start=koa_ft_map_addr_depth_end_14-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv2_interval_of_data; break;
    case(15): this->addr_data_start=koa_ft_map_addr_depth_end_15-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(16): this->addr_data_start=koa_ft_map_addr_depth_end_16-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv3_interval_of_data; break;
    case(17): this->addr_data_start=koa_ft_map_addr_depth_end_17-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv4_interval_of_data; break;
    case(18): this->addr_data_start=koa_ft_map_addr_depth_end_18-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv5_interval_of_data; break;
    case(19): this->addr_data_start=koa_ft_map_addr_depth_end_19-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    // c_x, r_x
    case(20): this->addr_data_start=koa_ft_map_c_x_addr_depth_end-koa_ft_map_c_x_addr_depth_start; this->addr_interval_of_data = koa_ft_map_c_x_addr_interval_of_data; break;
    case(21): this->addr_data_start=koa_ft_map_r_x_addr_depth_end-koa_ft_map_r_x_addr_depth_start; this->addr_interval_of_data = koa_ft_map_r_x_addr_interval_of_data; break;
    
    case(22): this->addr_data_start=koa_ft_map_addr_depth_end_22-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    case(23): this->addr_data_start=koa_ft_map_addr_depth_end_23-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_cls_conv1_interval_of_data; break;
    case(24): this->addr_data_start=koa_ft_map_addr_depth_end_24-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_cls_conv2_interval_of_data; break;
    case(25): this->addr_data_start=koa_ft_map_addr_depth_end_25-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_cls_conv3_interval_of_data; break;
    case(26): this->addr_data_start=koa_ft_map_addr_depth_end_26-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_cls_score_interval_of_data; break;
    case(27): this->addr_data_start=koa_ft_map_addr_depth_end_27-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_ctr_score_interval_of_data; break;
    case(28): this->addr_data_start=koa_ft_map_addr_depth_end_28-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_conv6_interval_of_data; break;
    case(29): this->addr_data_start=koa_ft_map_addr_depth_end_29-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_bbox_conv1_interval_of_data; break;
    case(30): this->addr_data_start=koa_ft_map_addr_depth_end_30-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_bbox_conv2_interval_of_data; break;
    case(31): this->addr_data_start=koa_ft_map_addr_depth_end_31-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_bbox_conv3_interval_of_data; break;
    case(32): this->addr_data_start=koa_ft_map_addr_depth_end_32-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_offset_score_interval_of_data; break;
  
  
    case(100): this->addr_data_start=koa_ft_map_addr_depth_end_100-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_test_conv_input_interval_of_data; break;
    case(101): this->addr_data_start=koa_ft_map_addr_depth_end_101-koa_ft_map_addr_depth_start; this->addr_interval_of_data = koa_ft_map_test_conv_output_interval_of_data; break;
  
  }
}



//WSram
int WSramAddressGenerator::AddressSOfWeightBundle(int bundle_idx){
  return this->addr_s_start + bundle_idx/kSramAccessBytes;
}
int WSramAddressGenerator::AddressDepthOfWeightBundle(int bundle_idx){
  return this->addr_depth_start + bundle_idx/kSramAccessBytes;
}
int WSramAddressGenerator::AddressPrkDepthOfWeightBundle(int bundle_idx){
  return this->addr_prk_depth_start + bundle_idx/kSramAccessBytes;
}

int WSramAddressGenerator::AddressDataOfWeightBundle(int bundle_idx){
  return this->addr_data_start + bundle_idx * this->addr_interval_of_data;
}
int WSramAddressGenerator::AddressDataEndOfWeightBundle(int bundle_idx){
  return this->addr_data_start + (bundle_idx+1) * this->addr_interval_of_data;
}

int WSramAddressGenerator::AddressPrkOfWeightBundle(int bundle_idx){
  return this->addr_prk_start + bundle_idx * this->addr_interval_of_prk;
}
int WSramAddressGenerator::AddressPrkEndOfWeightBundle(int bundle_idx){
  return this->addr_prk_start + (bundle_idx+1) * this->addr_interval_of_prk;
}

int WSramAddressGenerator::AddressBiasOfWeightBundle(){
  return this->addr_bias_start;
}
int WSramAddressGenerator::AddressBiasEndOfWeightBundle(){
  return this->addr_bias_end;
}


int WSramAddressGenerator::AddressZKernelDepthOfInputActivationBundle(int s){
  return this->addr_depth_start + s;
}

int WSramAddressGenerator::AddressZKernelDataOfInputActivationBundle(int s){
  return this->addr_data_start + s*6*specs::kWBundleMaxSize*2/kDramAccessBytes;
}

int WSramAddressGenerator::AddressZKernelDataEndOfInputActivationBundle(int s){
  return this->addr_data_start + (s+1)*6*specs::kWBundleMaxSize*2/kDramAccessBytes;
}


void WSramAddressGenerator::Calibrate(int weight_idx){
  this->addr_s_start = 0;
  switch (weight_idx){
    case(1): case(12): //conv1
      this->addr_depth_start      = kconv1_addr_depth_start - kconv1_addr_s_start;
      this->addr_prk_depth_start  = kconv1_addr_prkdepth_start - kconv1_addr_s_start;
      this->addr_data_start       = kconv1_addr_data_start - kconv1_addr_s_start;
      this->addr_prk_start        = kconv1_addr_prk_start - kconv1_addr_s_start;
      this->addr_bias_start       = kconv1_addr_bias_start-kconv1_addr_s_start;
      this->addr_bias_end         = kconv1_addr_bias_end-kconv1_addr_s_start;
      this->addr_interval_of_data = kconv1_addr_interval_of_data;
      this->addr_interval_of_prk  = kconv1_addr_interval_of_prk;
      break;
    case(2): case(13): //conv2
      this->addr_depth_start      = kconv2_addr_depth_start - kconv2_addr_s_start;
      this->addr_prk_depth_start  = kconv2_addr_prkdepth_start - kconv2_addr_s_start;
      this->addr_data_start       = kconv2_addr_data_start - kconv2_addr_s_start;
      this->addr_prk_start        = kconv2_addr_prk_start - kconv2_addr_s_start;
      this->addr_bias_start       = kconv2_addr_bias_start-kconv2_addr_s_start;
      this->addr_bias_end         = kconv2_addr_bias_end-kconv2_addr_s_start;
      this->addr_interval_of_data = kconv2_addr_interval_of_data;
      this->addr_interval_of_prk  = kconv2_addr_interval_of_prk;
      break;
    case(4): case(15): //conv3
      this->addr_depth_start      = kconv3_addr_depth_start - kconv3_addr_s_start;
      this->addr_prk_depth_start  = kconv3_addr_prkdepth_start - kconv3_addr_s_start;
      this->addr_data_start       = kconv3_addr_data_start - kconv3_addr_s_start;
      this->addr_prk_start        = kconv3_addr_prk_start - kconv3_addr_s_start;
      this->addr_bias_start       = kconv3_addr_bias_start-kconv3_addr_s_start;
      this->addr_bias_end         = kconv3_addr_bias_end-kconv3_addr_s_start;
      this->addr_interval_of_data = kconv3_addr_interval_of_data;
      this->addr_interval_of_prk  = kconv3_addr_interval_of_prk;
      break;
    case(6): case(17): //conv4
      this->addr_depth_start      = kconv4_addr_depth_start - kconv4_addr_s_start;
      this->addr_prk_depth_start  = kconv4_addr_prkdepth_start - kconv4_addr_s_start;
      this->addr_data_start       = kconv4_addr_data_start - kconv4_addr_s_start;
      this->addr_prk_start        = kconv4_addr_prk_start - kconv4_addr_s_start;
      this->addr_bias_start       = kconv4_addr_bias_start-kconv4_addr_s_start;
      this->addr_bias_end         = kconv4_addr_bias_end-kconv4_addr_s_start;
      this->addr_interval_of_data = kconv4_addr_interval_of_data;
      this->addr_interval_of_prk  = kconv4_addr_interval_of_prk;
      break;
    case(7): case(18): //conv5
      this->addr_depth_start      = kconv5_addr_depth_start - kconv5_addr_s_start;
      this->addr_prk_depth_start  = kconv5_addr_prkdepth_start - kconv5_addr_s_start;
      this->addr_data_start       = kconv5_addr_data_start - kconv5_addr_s_start;
      this->addr_prk_start        = kconv5_addr_prk_start - kconv5_addr_s_start;
      this->addr_bias_start       = kconv5_addr_bias_start-kconv5_addr_s_start;
      this->addr_bias_end         = kconv5_addr_bias_end-kconv5_addr_s_start;
      this->addr_interval_of_data = kconv5_addr_interval_of_data;
      this->addr_interval_of_prk  = kconv5_addr_interval_of_prk;
      break;
    case(8): case(19): //conv6
      this->addr_depth_start      = kconv6_addr_depth_start - kconv6_addr_s_start;
      this->addr_prk_depth_start  = kconv6_addr_prkdepth_start - kconv6_addr_s_start;
      this->addr_data_start       = kconv6_addr_data_start - kconv6_addr_s_start;
      this->addr_prk_start        = kconv6_addr_prk_start - kconv6_addr_s_start;
      this->addr_bias_start       = kconv6_addr_bias_start-kconv6_addr_s_start;
      this->addr_bias_end         = kconv6_addr_bias_end-kconv6_addr_s_start;
      this->addr_interval_of_data = kconv6_addr_interval_of_data;
      this->addr_interval_of_prk  = kconv6_addr_interval_of_prk;
      break;
    case(9): //c_z_k
      this->addr_depth_start      = kc_z_k_addr_depth_start - kc_z_k_addr_s_start;
      this->addr_prk_depth_start  = kc_z_k_addr_prkdepth_start - kc_z_k_addr_s_start;
      this->addr_data_start       = kc_z_k_addr_data_start - kc_z_k_addr_s_start;
      this->addr_prk_start        = kc_z_k_addr_prk_start - kc_z_k_addr_s_start;
      this->addr_bias_start       = kc_z_k_addr_bias_start-kc_z_k_addr_s_start;
      this->addr_bias_end         = kc_z_k_addr_bias_end-kc_z_k_addr_s_start;
      this->addr_interval_of_data = kc_z_k_addr_interval_of_data;
      this->addr_interval_of_prk  = kc_z_k_addr_interval_of_prk;
      break;
    case(10): //r_z_k
      this->addr_depth_start      = kr_z_k_addr_depth_start - kr_z_k_addr_s_start;
      this->addr_prk_depth_start  = kr_z_k_addr_prkdepth_start - kr_z_k_addr_s_start;
      this->addr_data_start       = kr_z_k_addr_data_start - kr_z_k_addr_s_start;
      this->addr_prk_start        = kr_z_k_addr_prk_start - kr_z_k_addr_s_start;
      this->addr_bias_start       = kr_z_k_addr_bias_start-kr_z_k_addr_s_start;
      this->addr_bias_end         = kr_z_k_addr_bias_end-kr_z_k_addr_s_start;
      this->addr_interval_of_data = kr_z_k_addr_interval_of_data;
      this->addr_interval_of_prk  = kr_z_k_addr_interval_of_prk;
      break;
    case(20): //c_x
      this->addr_depth_start      = kc_x_addr_depth_start - kc_x_addr_s_start;
      this->addr_prk_depth_start  = kc_x_addr_prkdepth_start - kc_x_addr_s_start;
      this->addr_data_start       = kc_x_addr_data_start - kc_x_addr_s_start;
      this->addr_prk_start        = kc_x_addr_prk_start - kc_x_addr_s_start;
      this->addr_bias_start       = kc_x_addr_bias_start-kc_x_addr_s_start;
      this->addr_bias_end         = kc_x_addr_bias_end-kc_x_addr_s_start;
      this->addr_interval_of_data = kc_x_addr_interval_of_data;
      this->addr_interval_of_prk  = kc_x_addr_interval_of_prk;
      break;
    case(21): //r_x
      this->addr_depth_start      = kr_x_addr_depth_start - kr_x_addr_s_start;
      this->addr_prk_depth_start  = kr_x_addr_prkdepth_start - kr_x_addr_s_start;
      this->addr_data_start       = kr_x_addr_data_start - kr_x_addr_s_start;
      this->addr_prk_start        = kr_x_addr_prk_start - kr_x_addr_s_start;
      this->addr_bias_start       = kr_x_addr_bias_start-kr_x_addr_s_start;
      this->addr_bias_end         = kr_x_addr_bias_end-kr_x_addr_s_start;
      this->addr_interval_of_data = kr_x_addr_interval_of_data;
      this->addr_interval_of_prk  = kr_x_addr_interval_of_prk;
      break;
    case(23): //cls_conv1
      this->addr_depth_start      = kcls_conv1_addr_depth_start - kcls_conv1_addr_s_start;
      this->addr_prk_depth_start  = kcls_conv1_addr_prkdepth_start - kcls_conv1_addr_s_start;
      this->addr_data_start       = kcls_conv1_addr_data_start - kcls_conv1_addr_s_start;
      this->addr_prk_start        = kcls_conv1_addr_prk_start - kcls_conv1_addr_s_start;
      this->addr_bias_start       = kcls_conv1_addr_bias_start-kcls_conv1_addr_s_start;
      this->addr_bias_end         = kcls_conv1_addr_bias_end-kcls_conv1_addr_s_start;
      this->addr_interval_of_data = kcls_conv1_addr_interval_of_data;
      this->addr_interval_of_prk  = kcls_conv1_addr_interval_of_prk;
      break;
    case(24): //cls_conv2
      this->addr_depth_start      = kcls_conv2_addr_depth_start - kcls_conv2_addr_s_start;
      this->addr_prk_depth_start  = kcls_conv2_addr_prkdepth_start - kcls_conv2_addr_s_start;
      this->addr_data_start       = kcls_conv2_addr_data_start - kcls_conv2_addr_s_start;
      this->addr_prk_start        = kcls_conv2_addr_prk_start - kcls_conv2_addr_s_start;
      this->addr_bias_start       = kcls_conv2_addr_bias_start-kcls_conv2_addr_s_start;
      this->addr_bias_end         = kcls_conv2_addr_bias_end-kcls_conv2_addr_s_start;
      this->addr_interval_of_data = kcls_conv2_addr_interval_of_data;
      this->addr_interval_of_prk  = kcls_conv2_addr_interval_of_prk;
      break;
    case(25): //cls_conv3
      this->addr_depth_start      = kcls_conv3_addr_depth_start - kcls_conv3_addr_s_start;
      this->addr_prk_depth_start  = kcls_conv3_addr_prkdepth_start - kcls_conv3_addr_s_start;
      this->addr_data_start       = kcls_conv3_addr_data_start - kcls_conv3_addr_s_start;
      this->addr_prk_start        = kcls_conv3_addr_prk_start - kcls_conv3_addr_s_start;
      this->addr_bias_start       = kcls_conv3_addr_bias_start-kcls_conv3_addr_s_start;
      this->addr_bias_end         = kcls_conv3_addr_bias_end-kcls_conv3_addr_s_start;
      this->addr_interval_of_data = kcls_conv3_addr_interval_of_data;
      this->addr_interval_of_prk  = kcls_conv3_addr_interval_of_prk;
      break;
    case(26): //cls_score
      this->addr_depth_start      = kcls_score_addr_depth_start - kcls_score_addr_s_start;
      this->addr_prk_depth_start  = kcls_score_addr_prkdepth_start - kcls_score_addr_s_start;
      this->addr_data_start       = kcls_score_addr_data_start - kcls_score_addr_s_start;
      this->addr_prk_start        = kcls_score_addr_prk_start - kcls_score_addr_s_start;
      this->addr_bias_start       = kcls_score_addr_bias_start-kcls_score_addr_s_start;
      this->addr_bias_end         = kcls_score_addr_bias_end-kcls_score_addr_s_start;
      this->addr_interval_of_data = kcls_score_addr_interval_of_data;
      this->addr_interval_of_prk  = kcls_score_addr_interval_of_prk;
      break;
    case(27): //ctr_score
      this->addr_depth_start      = kctr_score_addr_depth_start - kctr_score_addr_s_start;
      this->addr_prk_depth_start  = kctr_score_addr_prkdepth_start - kctr_score_addr_s_start;
      this->addr_data_start       = kctr_score_addr_data_start - kctr_score_addr_s_start;
      this->addr_prk_start        = kctr_score_addr_prk_start - kctr_score_addr_s_start;
      this->addr_bias_start       = kctr_score_addr_bias_start-kctr_score_addr_s_start;
      this->addr_bias_end         = kctr_score_addr_bias_end-kctr_score_addr_s_start;
      this->addr_interval_of_data = kctr_score_addr_interval_of_data;
      this->addr_interval_of_prk  = kctr_score_addr_interval_of_prk;
      break;
    case(29): //bbox_conv1
      this->addr_depth_start      = kbbox_conv1_addr_depth_start - kbbox_conv1_addr_s_start;
      this->addr_prk_depth_start  = kbbox_conv1_addr_prkdepth_start - kbbox_conv1_addr_s_start;
      this->addr_data_start       = kbbox_conv1_addr_data_start - kbbox_conv1_addr_s_start;
      this->addr_prk_start        = kbbox_conv1_addr_prk_start - kbbox_conv1_addr_s_start;
      this->addr_bias_start       = kbbox_conv1_addr_bias_start-kbbox_conv1_addr_s_start;
      this->addr_bias_end         = kbbox_conv1_addr_bias_end-kbbox_conv1_addr_s_start;
      this->addr_interval_of_data = kbbox_conv1_addr_interval_of_data;
      this->addr_interval_of_prk  = kbbox_conv1_addr_interval_of_prk;
      break;
    case(30): //bbox_conv2
      this->addr_depth_start      = kbbox_conv2_addr_depth_start - kbbox_conv2_addr_s_start;
      this->addr_prk_depth_start  = kbbox_conv2_addr_prkdepth_start - kbbox_conv2_addr_s_start;
      this->addr_data_start       = kbbox_conv2_addr_data_start - kbbox_conv2_addr_s_start;
      this->addr_prk_start        = kbbox_conv2_addr_prk_start - kbbox_conv2_addr_s_start;
      this->addr_bias_start       = kbbox_conv2_addr_bias_start-kbbox_conv2_addr_s_start;
      this->addr_bias_end         = kbbox_conv2_addr_bias_end-kbbox_conv2_addr_s_start;
      this->addr_interval_of_data = kbbox_conv2_addr_interval_of_data;
      this->addr_interval_of_prk  = kbbox_conv2_addr_interval_of_prk;
      break;
    case(31): //bbox_conv3
      this->addr_depth_start      = kbbox_conv3_addr_depth_start - kbbox_conv3_addr_s_start;
      this->addr_prk_depth_start  = kbbox_conv3_addr_prkdepth_start - kbbox_conv3_addr_s_start;
      this->addr_data_start       = kbbox_conv3_addr_data_start - kbbox_conv3_addr_s_start;
      this->addr_prk_start        = kbbox_conv3_addr_prk_start - kbbox_conv3_addr_s_start;
      this->addr_bias_start       = kbbox_conv3_addr_bias_start-kbbox_conv3_addr_s_start;
      this->addr_bias_end         = kbbox_conv3_addr_bias_end-kbbox_conv3_addr_s_start;
      this->addr_interval_of_data = kbbox_conv3_addr_interval_of_data;
      this->addr_interval_of_prk  = kbbox_conv3_addr_interval_of_prk;
      break;
    case(32): //offset_score
      this->addr_depth_start      = koffset_score_addr_depth_start - koffset_score_addr_s_start;
      this->addr_prk_depth_start  = koffset_score_addr_prkdepth_start - koffset_score_addr_s_start;
      this->addr_data_start       = koffset_score_addr_data_start - koffset_score_addr_s_start;
      this->addr_prk_start        = koffset_score_addr_prk_start - koffset_score_addr_s_start;
      this->addr_bias_start       = koffset_score_addr_bias_start-koffset_score_addr_s_start;
      this->addr_bias_end         = koffset_score_addr_bias_end-koffset_score_addr_s_start;
      this->addr_interval_of_data = koffset_score_addr_interval_of_data;
      this->addr_interval_of_prk  = koffset_score_addr_interval_of_prk;
      break;

    case(40): //c_z
      this->addr_depth_start      = 0;
      this->addr_data_start       = kc_z_addr_data_start - kc_z_addr_depth_start;
      break;
    case(41): //r_z
      this->addr_depth_start      = 0;
      this->addr_data_start       = kr_z_addr_data_start - kr_z_addr_depth_start;
      break;


    case(100):
      this->addr_depth_start      = ktest_conv_addr_depth_start - ktest_conv_addr_s_start;
      this->addr_prk_depth_start  = ktest_conv_addr_prkdepth_start - ktest_conv_addr_s_start;
      this->addr_data_start       = ktest_conv_addr_data_start - ktest_conv_addr_s_start;
      this->addr_prk_start        = ktest_conv_addr_prk_start - ktest_conv_addr_s_start;
      this->addr_bias_start       = ktest_conv_addr_bias_start-ktest_conv_addr_s_start;
      this->addr_bias_end         = ktest_conv_addr_bias_end-ktest_conv_addr_s_start;
      this->addr_interval_of_data = ktest_conv_addr_interval_of_data;
      this->addr_interval_of_prk  = ktest_conv_addr_interval_of_prk;
      break;
  }
}