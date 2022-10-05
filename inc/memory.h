#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <vector>
#include "specs.h"

constexpr int kDramAccessBits   {128};
constexpr int kDramAccessBytes  {kDramAccessBits/8};
constexpr int kSramAccessBits   {128};
constexpr int kSramAccessBytes  {kSramAccessBits/8};
constexpr int kByteOffset       {4};
constexpr int kByte             {8};

class DramMemory {
  public:
    DramMemory(){};
    DramMemory(int row_size, int col_size);

    void write(int addr, std::vector<int> data); // FIXME data type, 32bit?
    std::vector<int> read(int addr); // FIXME data type, 32bit?

    std::vector<std::vector<int>> mem_;
  private:
};

class SramMemory {
  public:
    SramMemory(){};
    SramMemory(int row_size, int col_size);

    void write(int addr, std::vector<int> data); // FIXME data type, 32bit?
    std::vector<int> read(int addr); // FIXME data type, 32bit?

    std::vector<std::vector<int>> mem_;
  private:
};

#endif // __MEMORY_H__