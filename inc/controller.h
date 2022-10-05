#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <vector>
#include <string>

#include "specs.h"

// modules
#include "../inc/compressor.h"
#include "../inc/memory_controller.h"
#include "../inc/max_pool.h"
#include "../inc/reducer.h"
#include "../inc/memory_controller.h"
#include "../inc/AIM/associative_index_matching_unit.h"
#include "../inc/PE/pe.h"


class Controller {
  public:
    enum OutputType {
      OA,
      WEIGHT,
      SCORE
    };

    Controller(int num_of_PE_set, int num_of_PE_in_set);

    void Test();
    void Init();
    void Track();
    void Clear();

    // multiple layers processing
    void ImgZAlexnet(bool log = false);
    void ImgXAlexnet(bool log = false);
    void TestXcorr();

    // DRAM initialization
    void InitDramOA(std::string);
    void InitDramWeight(std::string);
    

  private:
    // for temporary use
    std::vector<specs::ValidPositionPair> PseudoAIM(specs::InputActivationBundle, specs::WeightBundle);


    //////////////// data members
    int num_of_PE_set_;
    int num_of_PE_in_set_;

    // modules
    MemoryController mem_controller_;
    Compressor compressor_;
    MaxPool max_pool_;
    Reducer reducer_;
    std::vector<AssociativeIndexMatchingUnit> aims_;
    std::vector<std::vector<PE>> PEs_; // 2 dimension PEs
    
    // test
    std::vector<std::vector<std::vector<int>>> psum_;

    //////////////// functions



    // init steps
    void InitCompressInput(specs::RawData raw_data);

    

    // single layer processing
    void Convolution(specs::ConvConfig, OutputType type = OA, bool verbose = false);
    void MaxPool(specs::MaxPoolConfig);
    void ZK_Convolution(specs::ConvConfig, bool verbose = false);
    void XCorr(specs::ConvConfig, bool verbose = false);

    // for testing
    
    
    void TestConv(specs::ConvConfig);
    void TestMaxPool(specs::MaxPoolConfig);
    void ReadOA(int, int, std::string);
    void ReadWeight(int, int, std::string);
    void ReadZKernel(int, int, std::string);
    void UpdatePsum(specs::RawData);
    void PrintPsum(std::vector<int>, specs::ConvConfig, std::ostream& stream = std::cout);
};


#endif // __CONTROLLER_H__