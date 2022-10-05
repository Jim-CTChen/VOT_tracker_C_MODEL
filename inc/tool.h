#include <fstream>
#include <iostream>

#include "specs.h"
#include "../inc/memory_controller.h"

void InitConvConfig();

void PrintIABundle(specs::InputActivationBundle b, std::ostream& stream = std::cout);
void PrintWBundle(specs::WeightBundle b, std::ostream& stream = std::cout);
void PrintPartialSum(specs::RawData raw, std::ostream& stream = std::cout);
void PrintBias(std::vector<int> bias, std::ostream& stream = std::cout);
void PrintDram(MemoryController mem_ctrl, std::ostream& stream);
void PrintIaSram(MemoryController mem_ctrl, std::ostream& stream);
void PrintWSram(MemoryController mem_ctrl, std::ostream& stream);
void PrintValidPositionPairs(std::vector<specs::ValidPositionPair>, std::ostream& stream = std::cout);