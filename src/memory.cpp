#include "../inc/memory.h"
#include "../inc/specs.h"

#include <vector>

/*===========================
==========   DRAM   =========
=============================*/

// Read kDramAccessBytes data at a time.
DramMemory::DramMemory(int row_size, int col_size){
  std::vector<std::vector<int>> mem(row_size, std::vector<int>(col_size,0));
  this->mem_ = mem;
}

void DramMemory::write(int addr, std::vector<int> data){
  this->mem_[addr] = data;
}

std::vector<int> DramMemory::read(int addr){
  return this->mem_[addr];
}

/*===========================
==========   SRAM   =========
=============================*/

// Read kSramAccessBytes data at a time.
SramMemory::SramMemory(int row_size, int col_size){
  std::vector<std::vector<int>> mem(row_size, std::vector<int>(col_size,0));
  this->mem_ = mem;
}

void SramMemory::write(int addr, std::vector<int> data){
  this->mem_[addr] = data;
}

std::vector<int> SramMemory::read(int addr){
  return this->mem_[addr];
}