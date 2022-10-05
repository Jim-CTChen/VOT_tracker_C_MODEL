#include "../inc/memory_controller.h"
#include "../inc/address_config_v1.h"
#include "../inc/specs.h"
#include <vector>
#include <iostream>

#include <math.h>

MemoryController::MemoryController(){
  this->dram_ = DramMemory(koffset_score_addr_end, kDramAccessBytes);
  this->ia_sram_ = SramMemory(koa_ft_map_addr_end-koa_ft_map_addr_depth_start, kSramAccessBytes);
  this->w_sram_ = SramMemory(80000, kSramAccessBytes);
  this->depth_accumulator = std::vector<int>(6,0);
  this->depth_buffer = std::vector<std::vector<int>>(6,std::vector<int>(kDramAccessBytes,0));
  this->concat_buffer = std::vector<std::vector<int>>(6,std::vector<int>(kDramAccessBytes,0));

  this->fifo_Oa2Dram_ = std::vector<std::vector<int>>(1,std::vector<int>(kDramAccessBytes,0));
}


/*=========== Dram ==========*/
 
// Move whole Input Activation(img_z, img_x, out_ft_map) to Sram
void MemoryController::MoveInputActivationBundleSetFromDramToIaSram(int input_ft_map_idx){
  int addr_dram_depth_start, addr_dram_depth_end, addr_dram_data_start, addr_dram_end;
  int addr_sram_start = 0;

  // std::vector<int> convconfig_info = this->dram_.read(input_ft_map_idx);
  // this->ia_sram_.write(0,convconfig_info);

  std::tie(addr_dram_depth_start, addr_dram_depth_end, addr_dram_data_start, addr_dram_end)  = this->addr_dram_generator_.AddressStartAndEndOfInputActivationBundleSet(input_ft_map_idx);

  int addr_dram = addr_dram_depth_start;
  int addr_sram = addr_sram_start;

  std::vector<int> rdata(kDramAccessBytes,0);


  while (addr_dram < addr_dram_depth_end){
    rdata = this->dram_.read(addr_dram);
    this->ia_sram_.write(addr_sram, rdata);
    addr_dram++;
    addr_sram++;
  }
  addr_dram = addr_dram_data_start;
  while (addr_dram < addr_dram_end){
    rdata = this->dram_.read(addr_dram);
    this->ia_sram_.write(addr_sram, rdata);
    addr_dram++;
    addr_sram++;
  }
  
    // while (this->fifo_Dram2Sram_.full == false | addr_dram>=addr_dram_end){
    //   rdata = this->dram_.read(addr_dram);
    //   this->fifo_Dram2Sram_.write(rdata);
    //   addr_dram++;
    // }
    // while (this->fifo_Dram2Sram_.empty == false){
    //   this->ia_sram_.write(addr_sram, this->fifo_Dram2Sram_.read());
    //   addr_sram++;
    // }
  // for (int i=0; i<3; i++){
  //   std::fill(this->fifo_Dram2Sram_.data_buffers[i].begin(), this->fifo_Dram2Sram_.data_buffers[i].end(), 0);
  // }
}


// Move whole Weight to Sram
void MemoryController::MoveWeightBundleSetFromDramToWSram(int input_weight_idx){
  int addr_dram_start, addr_dram_end;
  int addr_sram_start = 0;
  std::tie(addr_dram_start, addr_dram_end)  = this->addr_dram_generator_.AddressStartAndEndOfWeightBundleSet(input_weight_idx);

  int addr_dram = addr_dram_start;
  int addr_sram = addr_sram_start;

  std::vector<int> rdata(kDramAccessBytes,0);

  while (addr_dram < addr_dram_end){
    rdata = this->dram_.read(addr_dram);
    this->w_sram_.write(addr_sram, rdata);
    addr_dram++;
    addr_sram++;
  }
  // while (addr_dram < addr_dram_end){
  //   while (this->fifo_Dram2Sram_.full == false | addr_dram >= addr_dram_end){
  //     rdata = this->dram_.read(addr_dram);
  //     this->fifo_Dram2Sram_.write(rdata);
  //     addr_dram++;
  //   }
  //   while (this->fifo_Dram2Sram_.empty == false){
  //     this->w_sram_.write(addr_sram, this->fifo_Dram2Sram_.read());
  //     addr_sram++;
  //   }
  // }
  // for (int i=0; i<3; i++){
  //   std::fill(this->fifo_Dram2Sram_.data_buffers[i].begin(), this->fifo_Dram2Sram_.data_buffers[i].end(), 0);
  // }
}


// Write out_ft_map back to Dram
void MemoryController::WriteInputActivationBundleToDram(specs::InputActivationBundle bundle, int bundle_idx){
  int addr, addr_depth, addr_data, addr_data_end;
  // int bundle_idx = this->output_bundle_size*bundle.h+bundle.w;

  this->addr_dram_generator_.CalibrateWriteAddress(this->output_ft_map_idx);
  addr_depth    = this->addr_dram_generator_.AddressDepthOfInputActivationBundle(bundle_idx);
  addr_data     = this->addr_dram_generator_.AddressDataOfInputActivationBundle(bundle_idx);
  addr_data_end = this->addr_dram_generator_.AddressDataEndOfInputActivationBundle(bundle_idx);

  // std::cout<<"this->output_ft_map_idx: "<<this->output_ft_map_idx<<std::endl;
  // std::cout<<"addr_depth: "<<addr_depth<<std::endl;
  // std::cout<<"addr_data: "<<addr_data<<std::endl;
  // std::cout<<"addr_data_end: "<<addr_data_end<<std::endl;
  // std::cout<<"bundle.depth: "<<bundle.depth<<std::endl;
  // std::cout<<"bundle.data[0]: "<<bundle.data[0]<<std::endl;


  std::vector<int> temp(kDramAccessBytes,0);
  std::vector<int> wdata(kDramAccessBytes,0);

  temp = this->dram_.read(addr_depth);

  int col_offset = bundle_idx % kDramAccessBytes;
  temp[col_offset] = bundle.depth;
  this->dram_.write(addr_depth, temp);

  addr = addr_data;
  int w_ptr = 0;
  int max_write;
  while (addr < addr_data_end && w_ptr < bundle.depth){
    max_write = (bundle.depth-w_ptr>kDramAccessBytes/2)? kDramAccessBytes/2 : bundle.depth-w_ptr;
    for (int i=0; i<max_write; i++){
      wdata[2*i]   = bundle.data[w_ptr+i];
      wdata[2*i+1] = bundle.channel_idx[w_ptr+i];
    }
    w_ptr += max_write;
    this->dram_.write(addr, wdata);
    addr++;
  }

  // while (addr < addr_data_end){
  //   w_ptr = 0;
  //   while (this->fifo_Oa2Dram_.full == false | addr>=addr_data_end | w_ptr>bundle.depth){
  //     std::fill(wdata.begin(), wdata.end(), 0);
  //     for (int i=0; i<kDramAccessBytes/2; i++){
  //       wdata[2*i]   = bundle.data[w_ptr+i];
  //       wdata[2*i+1] = bundle.channel_idx[w_ptr+i];
  //     }
  //     w_ptr += kDramAccessBytes/2;
  //     this->fifo_Oa2Dram_.write(wdata);
  //   }
  //   while (this->fifo_Oa2Dram_.empty == false){
  //     this->dram_.write(addr, this->fifo_Oa2Dram_.read());
  //     addr++;
  //   }
  // }
  // for (int i=0; i<3; i++){
  //   std::fill(this->fifo_Oa2Dram_.data_buffers[i].begin(), this->fifo_Oa2Dram_.data_buffers[i].end(), 0);
  // }
}


// Write result to Dram
void MemoryController::WriteResultToDram(specs::InputActivationBundle bundle, int bundle_idx){
  int addr, addr_data;
  // int bundle_idx = this->output_bundle_size*bundle.h+bundle.w;

  this->addr_dram_generator_.CalibrateWriteAddress(this->output_ft_map_idx);
  addr_data     = this->addr_dram_generator_.AddressResultDataOfInputActivationBundle(bundle_idx);


  addr = addr_data;
  int start_ptr = (bundle_idx%2==0)? 0 : kDramAccessBytes/2;
  for (int i=0; i<bundle.depth; i++){
    this->fifo_Oa2Dram_[0][start_ptr+2*i]   = bundle.data[i];
    this->fifo_Oa2Dram_[0][start_ptr+2*i+1] = bundle.channel_idx[i];
  }
  
  if (bundle_idx%2==1 | (bundle.h==16&&bundle.w==16)){
    this->dram_.write(addr, this->fifo_Oa2Dram_[0]);
    std::fill(this->fifo_Oa2Dram_[0].begin(), this->fifo_Oa2Dram_[0].end(), 0);
  }
}

// Write z kernel to Dram
void MemoryController::WriteZKernelToDram(specs::InputActivationBundle bundle){
  int addr, addr_depth, addr_data, addr_data_end;
  // int bundle_idx = this->output_bundle_size*bundle.h+bundle.w;

  this->addr_dram_generator_.CalibrateWriteAddress(this->output_ft_map_idx);
  int s = bundle.w;
  addr_depth    = this->addr_dram_generator_.AddressZKernelDepthOfInputActivationBundle(s);
  addr_data     = this->addr_dram_generator_.AddressZKernelDataOfInputActivationBundle(s);
  addr_data_end = this->addr_dram_generator_.AddressZKernelDataEndOfInputActivationBundle(s);


  std::vector<int> wdata(kDramAccessBytes,0);
  int w_ptr, start_ptr;

  addr = addr_data;

  //concate dc at tail and head
  addr += this->depth_accumulator[s]/(kDramAccessBytes/2);
  start_ptr = this->depth_accumulator[s]%(kDramAccessBytes/2);
  w_ptr = 0;
  if (start_ptr!=0){
    // std::cout<< "depth_accumulator[s] "<<depth_accumulator[s]<<std::endl;
    // std::cout<< "addr "<< addr <<std::endl;
    // for (auto x:concat_buffer[s]){
    //   std::cout<<x<< " ";
    // }
    // std::cout<<std::endl;
    while(start_ptr < kDramAccessBytes/2 && w_ptr<bundle.depth){
      this->concat_buffer[s][2*start_ptr]   = (bundle.data[w_ptr]);
      this->concat_buffer[s][2*start_ptr+1] = (bundle.channel_idx[w_ptr]);
      w_ptr++;
      start_ptr++;
    }
    this->dram_.write(addr, this->concat_buffer[s]);
    addr++;
  }


  while (addr < addr_data_end && w_ptr<=bundle.depth-kDramAccessBytes/2){
    for (int i=0; i<kDramAccessBytes/2; i++){
      wdata[2*i]   = bundle.data[w_ptr+i];
      wdata[2*i+1] = bundle.channel_idx[w_ptr+i];
    }
    this->dram_.write(addr, wdata);
    addr++;
    w_ptr += kDramAccessBytes/2;
  }

  int tail_ptr=0;
  while(w_ptr<bundle.depth){
    this->concat_buffer[s][2*tail_ptr]   = (bundle.data[w_ptr]);
    this->concat_buffer[s][2*tail_ptr+1] = (bundle.channel_idx[w_ptr]);
    // std::cout << "w_ptr " << w_ptr << " tail_ptr " << tail_ptr <<std::endl;
    tail_ptr++;
    w_ptr++;
  }

  this->depth_accumulator[s]+=bundle.depth;

  this->depth_buffer[s][bundle.h] = bundle.depth;

  //write back depth at a time
  if (bundle.h==this->output_bundle_size-1){
    this->dram_.write(addr_depth, this->depth_buffer[s]);
    while(tail_ptr<kDramAccessBytes/2){
      this->concat_buffer[s][2*tail_ptr] = 0;
      this->concat_buffer[s][2*tail_ptr+1] = 0;
      tail_ptr++;
    }
    this->dram_.write(addr, this->concat_buffer[s]);
    this->depth_accumulator[s] = 0;
  }
}



/*=========== Sram ==========*/

specs::InputActivationBundle MemoryController::ReadInputActivationBundleFromIaSram(int bundle_idx){
  specs::InputActivationBundle bundle;
  std::vector<int> rdata(kSramAccessBytes,0);
  std::vector<int> rdepth(kSramAccessBytes,0);

  int col_offset  = bundle_idx % kSramAccessBytes;


  this->addr_ia_sram_generator_.Calibrate(this->input_ft_map_idx);
  int addr, addr_depth, addr_data, addr_data_end;
  addr_depth    = this->addr_ia_sram_generator_.AddressDepthOfInputActivationBundle(bundle_idx);
  if (this->input_ft_map_idx==0 | this->input_ft_map_idx==11){ //img_z, img_x
    addr_data     = this->addr_ia_sram_generator_.AddressDataOfInputActivationBundle(bundle_idx/2);
    addr_data_end = this->addr_ia_sram_generator_.AddressDataEndOfInputActivationBundle(bundle_idx/2);
  }
  else {
    addr_data     = this->addr_ia_sram_generator_.AddressDataOfInputActivationBundle(bundle_idx);
    addr_data_end = this->addr_ia_sram_generator_.AddressDataEndOfInputActivationBundle(bundle_idx);
  }

  // std::cout<<"addr_depth "<<addr_depth<<std::endl;
  // std::cout<<"addr_data "<<addr_data<<std::endl;
  // std::cout<<"addr_data_end "<<addr_data_end<<std::endl;


  rdepth = ia_sram_.read(addr_depth);
  bundle.depth = rdepth[col_offset];
  // std::cout<<"bundle.depth "<<bundle.depth<<std::endl;
  // std::cout<<"col_offset "<<col_offset<<std::endl;

  bundle.h     = bundle_idx / this->input_bundle_size;
  bundle.w     = bundle_idx % this->input_bundle_size;

  addr = addr_data;
  int r_ptr = 0;
  int max_read;
  int rdata_start= (this->input_ft_map_idx==0 | this->input_ft_map_idx==11)? 6*(bundle_idx%2) : 0; //For img_z, img_x
  while (addr < addr_data_end && r_ptr<bundle.depth){
    // Read kSramAccessBits-bit data at a time.
    // 4 for data, 4 for c-idx.
    max_read = (bundle.depth-r_ptr > (kSramAccessBytes)/2)? kSramAccessBytes/2 : bundle.depth-r_ptr;
    rdata = ia_sram_.read(addr);
    for (int i=0; i<max_read; i++){ 
      bundle.data.push_back(rdata[rdata_start+2*i]);
      bundle.channel_idx.push_back(rdata[rdata_start+2*i+1]);
    }
    addr++;
    r_ptr += max_read;
  }
  return bundle;
}

specs::WeightBundle MemoryController::ReadWeightBundleFromWSram(int bundle_idx){
  specs::WeightBundle bundle;
  std::vector<int> rs;
  std::vector<int> rdepth;
  std::vector<int> rprk_depth;
  std::vector<int> rdata;
  
  int col_offset  = bundle_idx % kSramAccessBytes;

  this->addr_w_sram_generator_.Calibrate(this->weight_idx);
  int addr;
  int addr_s         = this->addr_w_sram_generator_.AddressSOfWeightBundle(bundle_idx);
  int addr_depth     = this->addr_w_sram_generator_.AddressDepthOfWeightBundle(bundle_idx);
  int addr_prk_depth = this->addr_w_sram_generator_.AddressPrkDepthOfWeightBundle(bundle_idx);
  int addr_data      = this->addr_w_sram_generator_.AddressDataOfWeightBundle(bundle_idx);
  int addr_data_end  = this->addr_w_sram_generator_.AddressDataEndOfWeightBundle(bundle_idx);
  int addr_prk       = this->addr_w_sram_generator_.AddressPrkOfWeightBundle(bundle_idx);
  int addr_prk_end   = this->addr_w_sram_generator_.AddressPrkEndOfWeightBundle(bundle_idx);

  rs         = this->w_sram_.read(addr_s);
  rdepth     = this->w_sram_.read(addr_depth);
  rprk_depth = this->w_sram_.read(addr_prk_depth);

  bundle.s          = rs[col_offset];
  bundle.depth      = rdepth[col_offset];
  bundle.prk_depth  = rprk_depth[col_offset];


  addr = addr_data;
  int r_ptr=0;
  int max_read;
  while (addr < addr_data_end && r_ptr<bundle.depth){
    // Read kSramAccessBits-bit data at a time. 
    // 4 for data, 4 for c-idx.
    max_read = (bundle.depth-r_ptr) > (kSramAccessBytes/2)? kSramAccessBytes/2 : bundle.depth-r_ptr;
    rdata = this->w_sram_.read(addr);
    for (int i=0; i<max_read; i++){ //loop unroll
      bundle.data.push_back(rdata[2*i]);
      bundle.channel_idx.push_back(rdata[2*i+1]);
    }
    addr++;
    r_ptr+=max_read;
  }

  addr = addr_prk;
  r_ptr = 0;
  while (addr < addr_prk_end && r_ptr<bundle.prk_depth){
    // Read kSramAccessBits-bit data at a time.
    // 2 for pos_ptr, 2 for r_idx, 2 for k_idx
    max_read = (bundle.prk_depth-r_ptr) > (kSramAccessBytes/3)? kSramAccessBytes/3 : bundle.prk_depth-r_ptr;
    rdata = this->w_sram_.read(addr);
    for (int i=0; i<max_read; i++){ //loop unroll
      bundle.pos_ptr.push_back(rdata[3*i]);
      bundle.r_idx.push_back(rdata[3*i+1]);
      bundle.k_idx.push_back(rdata[3*i+2]);
    }
    addr++;
    r_ptr+=max_read;
  }
  return bundle;
}

std::vector<int> MemoryController::ReadWeightBiasFromWSram(int weight_idx){
  std::vector<int> bias;
  this->addr_w_sram_generator_.Calibrate(this->weight_idx);
  int addr_bias       = this->addr_w_sram_generator_.AddressBiasOfWeightBundle();
  int addr_bias_end   = this->addr_w_sram_generator_.AddressBiasEndOfWeightBundle();
  int addr;

  addr = addr_bias;
  std::vector<int> rbias;

  while (addr<addr_bias_end){
    rbias = this->w_sram_.read(addr);
    for (int i=0; i<(kSramAccessBytes/2); i++){ //assume bias size%16=0
      bias.push_back(rbias[i]);
    }
    addr++;
  }
  return bias;
}


std::vector<specs::WeightBundle> MemoryController::ReadZKernelBundleSetFromWSram(int s){
  int addr, addr_depth, addr_data, addr_data_end, addr_previous;

  this->addr_w_sram_generator_.Calibrate(this->weight_idx);
  addr_depth    = this->addr_w_sram_generator_.AddressZKernelDepthOfInputActivationBundle(s);
  addr_data     = this->addr_w_sram_generator_.AddressZKernelDataOfInputActivationBundle(s);
  addr_data_end = this->addr_w_sram_generator_.AddressZKernelDataEndOfInputActivationBundle(s);

  std::vector<specs::WeightBundle> bundle_set;
  specs::WeightBundle bundle;
  std::vector<int> rdepth;
  // std::vector<int> r_buffer;
  std::vector<int> rdata;


  rdepth = this->w_sram_.read(addr_depth);
  std::vector<int> depth_accumulator(6,0);
  int r_count=0;
  int bundle_num=1;
  int bundle_count=0;
  // int r_start=0;
  int r_ptr=0;
  int end_addr, max_read;
  int b_depth;
  int p,r;

  // r_buffer.push_back(0);
  while (r_count<6){
    depth_accumulator[r_count] = (r_count>0)? depth_accumulator[r_count-1]+rdepth[r_count] : rdepth[r_count];
    
    if (depth_accumulator[r_count]/specs::kWBundleMaxSize>0){
      // r_buffer.push_back(r_count);
      bundle_num++;
      depth_accumulator[r_count] = depth_accumulator[r_count]%specs::kWBundleMaxSize;
    }
    r_count++;
  }
  // r_buffer.push_back(5);

  addr = addr_data;
  addr_previous = addr;

  while (bundle_count<bundle_num){
    p=0;
    r_count = bundle_count;
    //data
    end_addr = (bundle_count+1==bundle_num)? depth_accumulator[5]/(kDramAccessBytes/2)+(depth_accumulator[5] % (kDramAccessBytes/2) != 0)
                                                :specs::kWBundleMaxSize/(kSramAccessBytes/2);
    b_depth = (bundle_count+1==bundle_num)? depth_accumulator[5] :specs::kWBundleMaxSize;
    r_ptr=0;
    while (addr < addr_previous + end_addr){
      rdata = w_sram_.read(addr);
      max_read = (b_depth-r_ptr > (kSramAccessBytes)/2)? kSramAccessBytes/2 : b_depth-r_ptr;
      for (int i=0; i<max_read; i++){ //loop unroll
        bundle.data.push_back(rdata[2*i]);
        bundle.channel_idx.push_back(rdata[2*i+1]);

        r = p>=depth_accumulator[r_count]? r_count+1:r_count;

        bundle.r_idx.push_back(r);
        bundle.k_idx.push_back(rdata[2*i+1]);
        bundle.pos_ptr.push_back(p);
        p++;
      }
      addr++;
      r_ptr += max_read;
    }
    addr_previous = addr;

    //prk
    // bundle.pos_ptr.push_back(0);
    // while (r_start<r_buffer[bundle_count+1]){
    //   bundle.r_idx.push_back(r_start);
    //   bundle.k_idx.push_back(0);
    //   bundle.pos_ptr.push_back(depth_accumulator[r_start]);
    //   r_start++;
    // }
    // bundle.r_idx.push_back(r_start);
    // bundle.k_idx.push_back(0);

    bundle.depth = (bundle_count==bundle_num-1)? depth_accumulator[5]:specs::kWBundleMaxSize;
    // bundle.prk_depth = r_buffer[bundle_count+1]-r_buffer[bundle_count]+1;
    bundle.prk_depth = p;
    bundle.s = s;

    bundle_set.push_back(bundle);

    bundle.data.clear();
    bundle.channel_idx.clear();
    bundle.pos_ptr.clear();
    bundle.r_idx.clear();
    bundle.k_idx.clear();

    // r_start = r_buffer[bundle_count+1];
    bundle_count++;
  }
  return bundle_set;
}

std::vector<int> MemoryController::ReadConvConfigFromSram(){
  std::vector<int> convconfig_info = this->ia_sram_.read(0);
  return convconfig_info;
}


void MemoryController::WeightCalibrate(int weight_idx){
  this->weight_idx = weight_idx;
}

void MemoryController::InputCalibrate(int input_ft_map_idx){
  this->input_ft_map_idx = input_ft_map_idx;
  switch (input_ft_map_idx){
    case(0): this->input_bundle_size = 127; break;
    case(1): this->input_bundle_size = 63; break;
    case(2): this->input_bundle_size = 61; break;
    case(3): this->input_bundle_size = 30; break;
    case(4): this->input_bundle_size = 28; break;
    case(5): this->input_bundle_size = 14; break;
    case(6): this->input_bundle_size = 12; break;
    case(7): this->input_bundle_size = 10; break;
    case(8): this->input_bundle_size = 8; break;
    case(9): this->input_bundle_size = 6; break;
    case(10): this->input_bundle_size = 6; break;
    case(11): this->input_bundle_size = 303; break;
    case(12): this->input_bundle_size = 151; break;
    case(13): this->input_bundle_size = 149; break;
    case(14): this->input_bundle_size = 74; break;
    case(15): this->input_bundle_size = 72; break;
    case(16): this->input_bundle_size = 36; break;
    case(17): this->input_bundle_size = 34; break;
    case(18): this->input_bundle_size = 32; break;
    case(19): this->input_bundle_size = 30; break;
    case(20): this->input_bundle_size = 28; break;
    case(21): this->input_bundle_size = 28; break;
    case(22): this->input_bundle_size = 23; break;
    case(23): this->input_bundle_size = 21; break;
    case(24): this->input_bundle_size = 19; break;
    case(25): this->input_bundle_size = 17; break;
    case(26): this->input_bundle_size = 17; break;
    case(27): this->input_bundle_size = 17; break;
    case(28): this->input_bundle_size = 23; break;
    case(29): this->input_bundle_size = 21; break;
    case(30): this->input_bundle_size = 19; break;
    case(31): this->input_bundle_size = 17; break;
    case(32): this->input_bundle_size = 17; break;


    case(100): this->input_bundle_size = 127; break;
    case(101): this->input_bundle_size = 63; break;
  }
}

void MemoryController::OutputCalibrate(int output_ft_map_idx){
  this->output_ft_map_idx = output_ft_map_idx;
  
  switch (output_ft_map_idx){
    case(0): this->output_bundle_size = 127; break;
    case(1): this->output_bundle_size = 63; break;
    case(2): this->output_bundle_size = 61; break;
    case(3): this->output_bundle_size = 30; break;
    case(4): this->output_bundle_size = 28; break;
    case(5): this->output_bundle_size = 14; break;
    case(6): this->output_bundle_size = 12; break;
    case(7): this->output_bundle_size = 10; break;
    case(8): this->output_bundle_size = 8; break;
    case(9): this->output_bundle_size = 6; break;
    case(10): this->output_bundle_size = 6; break;
    case(11): this->output_bundle_size = 303; break;
    case(12): this->output_bundle_size = 151; break;
    case(13): this->output_bundle_size = 149; break;
    case(14): this->output_bundle_size = 74; break;
    case(15): this->output_bundle_size = 72; break;
    case(16): this->output_bundle_size = 36; break;
    case(17): this->output_bundle_size = 34; break;
    case(18): this->output_bundle_size = 32; break;
    case(19): this->output_bundle_size = 30; break;
    case(20): this->output_bundle_size = 28; break;
    case(21): this->output_bundle_size = 28; break;
    case(22): this->output_bundle_size = 23; break;
    case(23): this->output_bundle_size = 21; break;
    case(24): this->output_bundle_size = 19; break;
    case(25): this->output_bundle_size = 17; break;
    case(26): this->output_bundle_size = 17; break;
    case(27): this->output_bundle_size = 17; break;
    case(28): this->output_bundle_size = 23; break;
    case(29): this->output_bundle_size = 21; break;
    case(30): this->output_bundle_size = 19; break;
    case(31): this->output_bundle_size = 17; break;
    case(32): this->output_bundle_size = 17; break;

    case(101): this->output_bundle_size = 6; break;
  }
}


