#ifndef __MEMORY_CONTROLLER__
#define __MEMORY_CONTROLLER__

#include "../inc/memory.h"
#include <tuple>

/*
  Assume SRAM can hold an entire bundle set
  So the interface of SRAM will be the index of bundle
  Since IA bundle and W bundle have different spec
  Split IA SRAM and W SRAM

  On the other hand
  DRAM hold all bundle sets
  So the interface of DRAM will be the index of bundle set
  Besides, assume that there's only one DRAM
*/

// wrap memory and access


/*===========================
==========   DRAM   =========
=============================*/

class DramAddressGenerator {
  public:
    DramAddressGenerator(){
      this->addr_depth_start      =0 ;
      this->addr_depth_start      =0 ;
      this->addr_data_start       =0 ;
      this->addr_interval_of_data =0 ;
      this->addr_s_start          =0 ;
      this->addr_prkdepth_start   =0 ;
      this->addr_prk_start        =0 ;
      this->addr_interval_of_prk  =0 ;
    };
    int addr_depth_start      ;
    int addr_data_start       ;
    int addr_interval_of_data ;
    int addr_s_start          ;
    int addr_prkdepth_start   ;
    int addr_prk_start        ;
    int addr_interval_of_prk  ;

    void CalibrateWriteAddress(int out_ft_map_idx);
    std::tuple<int,int,int,int> AddressStartAndEndOfInputActivationBundleSet(int input_ft_map_idx);
    std::tuple<int,int> AddressStartAndEndOfWeightBundleSet(int input_ft_map_idx);
    int AddressDepthOfInputActivationBundle(int bundle_idx);
    int AddressDataOfInputActivationBundle(int bundle_idx);
    int AddressDataEndOfInputActivationBundle(int bundle_idx);

    int AddressZKernelDepthOfInputActivationBundle(int s);
    int AddressZKernelDataOfInputActivationBundle(int s);
    int AddressZKernelDataEndOfInputActivationBundle(int s);

    int AddressResultDataOfInputActivationBundle(int bundle_idx);
};

/*===========================
==========   SRAM   =========
=============================*/

class IaSramAddressGenerator {
  public:
    IaSramAddressGenerator(){};
    int addr_depth_start      ;
    int addr_data_start       ;
    int addr_interval_of_data ;

    void Calibrate(int input_ft_map_idx);
    int AddressDepthOfInputActivationBundle(int bundle_idx);
    int AddressDataOfInputActivationBundle(int bundle_idx);
    int AddressDataEndOfInputActivationBundle(int bundle_idx);
};


class WSramAddressGenerator {
  public:
    WSramAddressGenerator(){};
    int addr_s_start          ;
    int addr_depth_start      ;
    int addr_prk_depth_start  ;
    int addr_data_start       ;
    int addr_prk_start        ;
    int addr_interval_of_data ;
    int addr_interval_of_prk  ;
    int addr_bias_start       ;
    int addr_bias_end         ;

    void Calibrate(int weight_idx);
    std::tuple<int,int> AddressStartAndEndOfWeightBundleSet(int bundle_set_idx);
    int AddressSOfWeightBundle(int bundle_idx);
    int AddressDepthOfWeightBundle(int bundle_idx);
    int AddressPrkDepthOfWeightBundle(int bundle_idx);
    int AddressDataOfWeightBundle(int bundle_idx);
    int AddressDataEndOfWeightBundle(int bundle_idx);
    int AddressPrkOfWeightBundle(int bundle_idx);
    int AddressPrkEndOfWeightBundle(int bundle_idx);
    int AddressBiasOfWeightBundle();
    int AddressBiasEndOfWeightBundle();

    int AddressZKernelDepthOfInputActivationBundle(int s);
    int AddressZKernelDataOfInputActivationBundle(int s);
    int AddressZKernelDataEndOfInputActivationBundle(int s);
};


class FIFO {
  public:
    FIFO();
    bool full;
    bool empty;
    int wptr;
    int rptr;
    std::vector<std::vector<int>> data_buffers;
    void write(std::vector<int> data);
    std::vector<int> read();
  private:
    int size;
};


class MemoryController {
  public:
    MemoryController();

    void InputCalibrate(int input_ft_map_idx);
    void OutputCalibrate(int output_ft_map_idx);
    void WeightCalibrate(int weight_idx);

    void MoveInputActivationBundleSetFromDramToIaSram(int intput_ft_map_idx);
    void MoveWeightBundleSetFromDramToWSram(int weight_idx);
    void WriteInputActivationBundleToDram(specs::InputActivationBundle ia_bundle, int bundle_idx);
    void WriteZKernelToDram(specs::InputActivationBundle w_bundle);
    void WriteResultToDram(specs::InputActivationBundle ia_bundle, int bundle_idx);

    specs::InputActivationBundle ReadInputActivationBundleFromIaSram(int ia_bundle_idx);
    specs::WeightBundle ReadWeightBundleFromWSram(int w_bundle_index);
    std::vector<int> ReadWeightBiasFromWSram(int weight_idx);
    std::vector<specs::WeightBundle> ReadZKernelBundleSetFromWSram(int s);
    std::vector<int> ReadConvConfigFromSram();


    //FIFO register
    std::vector<std::vector<int>> fifo_Oa2Dram_;

    // For storing Ia to Dram
    int input_ft_map_idx;
    int output_ft_map_idx;
    int weight_idx;

    int input_bundle_size;
    int output_bundle_size;
  
    //For z kernel
    std::vector<int> depth_accumulator;
    std::vector<std::vector<int>> depth_buffer;
    std::vector<std::vector<int>> concat_buffer;


    DramMemory dram_;
    SramMemory ia_sram_;
    SramMemory w_sram_;
    DramAddressGenerator   addr_dram_generator_;
    IaSramAddressGenerator addr_ia_sram_generator_;
    WSramAddressGenerator  addr_w_sram_generator_;
    // FIFO fifo_Dram2Sram_;
    // FIFO fifo_Oa2Dram_;
  private:
};

#endif // __MEMORY_WRAPPER_H__