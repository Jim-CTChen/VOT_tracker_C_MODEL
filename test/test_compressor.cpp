#include <iostream>
#include <vector>
#include <stdlib.h>

#include "specs.h"
#include "compressor.h"

using namespace std;

void PrintRaw(int h, int w, std::vector<int> raw, ostream& stream = cout) {
  stream << "Printing Raw Data By <h, w, k, value>: " << endl;
  for (int i = 0; i < raw.size(); i++) {
    stream << "<" << h << ", " << w << ", " << i << ", " << raw[i] << ">" << endl;
  }
}

void PrintIABundle(specs::InputActivationBundle b, ostream& stream = cout) {
  assert(b.data.size() == b.depth);
  assert(b.channel_idx.size() == b.depth);

  stream << "===== bundle info =====" << endl;
  stream << "\t" << "(h, w): (" << b.h  << ", " << b.w << ")" << endl;
  stream << "\t" << "size: " << b.depth << endl;
  for (int i = 0; i < b.depth; i++) {
    stream << "\t" << "<channel, data>: <" << b.channel_idx[i] << ", " << b.data[i] << ">" << endl;
  }
  stream << "======================" << endl;
}

class TestCompressor {
  public:
    void Test(int);
    Compressor my_compressor;
  private:
};

void TestCompressor::Test(int k) {
  int h = (rand() % 256);
  int w = (rand() % 256);
  
  // specs::RawData raw;
  vector<int> raw;

  for (int i = 0; i < k; i++) {
    int zero = rand() % 2;
    if (!zero) {
      int value = rand() % 256;
      raw.push_back(value);
    }
    else {
      raw.push_back(0);
    }
  }

  this->my_compressor.SetOAChannel(k);
  this->my_compressor.InputRaw(h, w, raw);
  specs::InputActivationBundle ia_bundle = this->my_compressor.Compress();
  PrintRaw(h, w, raw);
  PrintIABundle(ia_bundle);
}

int main() {
  TestCompressor tb;
  tb.Test(16);
}