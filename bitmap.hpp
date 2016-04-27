#ifndef BITMAP_HPP
#define BITMAP_HPP
#include <vector>
#include "utils.hpp"
using namespace std;

// Indices start from 0
struct BitmapRank {
  const int bits = sizeof(int)*8;
  vector<int> vec;
  vector<int> count;

  BitmapRank() {}

  void resize(int n) {
    vec.resize((n+bits-1)/bits);
    count.resize(vec.size());
  }

  void set(int i, bool b) {
    set_bit(vec[i/bits], i %bits, b);
  }

  void build_rank() {
    for (int i = 1; i < (int)vec.size(); ++i)
      count[i] = count[i-1] + popcnt(vec[i-1]);
  }

  int rank1(int i) const {
    return i < 0 ? 0 : count[i/bits] + popcnt(vec[i/bits] << (bits - i%bits - 1));
  }

  int rank1(int i, int j) const {
    return rank1(j) - rank1(i-1);
  }

  int rank0(int i) const {
    return i < 0 ? 0 : i - rank1(i) + 1;
  }

  int rank0(int i, int j) const {
    return rank0(j) - rank0(i-1);
  }
};

#endif
