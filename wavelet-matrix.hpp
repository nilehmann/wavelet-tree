/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * -----------------------------------------------------------------------------
 */
#include <vector>
#include <cstdio>
#include <algorithm>
#include "utils.hpp"
using namespace std;

typedef unsigned int uint;


template<class Bitmap>
struct WaveMatrix {
  uint height;
  vector<Bitmap> B;
  vector<int> z;
  int a;

  WaveMatrix(vector<int> &S) :
    WaveMatrix(S, *max_element(S.begin(), S.end()) + 1) {}

  WaveMatrix(vector<int> &S, int sigma)
    : height(log2(sigma - 1)),
      B(height), z(height) {
    for (uint l = 0; l < height; ++l) {
      B[l].resize(S.size());
      for (uint i = 0; i < S.size(); ++i)
        B[l].set(i, get_bit(S[i], height - l - 1));
      B[l].build_rank();

      auto it = stable_partition(S.begin(), S.end(), [=] (int c) {
          return not get_bit(c, height - l - 1);
        });
      z[l] = distance(S.begin(), it);
    }
  }

  int memory() const {
    int mem = 0;
    mem += sizeof(vector<Bitmap>);
    for (int l = 0; l < (int)B.size(); ++l)
      mem += B[l].memory();

    mem += sizeof(vector<int>);
    mem += z.capacity()*sizeof(int);

    mem += sizeof(uint);

    return mem;
  }

  // Find the k-th smallest element. The smallest element is k=1
  int quantile(int k, int i, int j) {
    int element = 0;
    for (uint l = 0; l < height; ++l) {
      int r = B[l].rank0(i, j);
      if (r >= k) {
        i = B[l].rank0(i-1);
        j = B[l].rank0(j) - 1;
      } else {
        i = z[l] + B[l].rank1(i-1);
        j = z[l] + B[l].rank1(j) - 1;
        k -= r;
        set_bit(element, height - l - 1);
      }
    }
    return element;
  }
  int getA() {return a;}

  // Count occurrences of number c until position i
  int rank(int c, int i) const {
    int p = -1;
    for (uint l = 0; l < height; ++l) {
      if (get_bit(c, height - l - 1)) {
        p = z[l] + B[l].rank1(p) - 1;
        i = z[l] + B[l].rank1(i) - 1;
      } else {
        p = B[l].rank0(p) - 1;
        i = B[l].rank0(i) - 1;
      }
    }
    return i - p;
  }

  // Count number of occurrences of numbers in the range [a, b]
  // present in the sequence at range [i, j], ie, if representing a grid it
  // counts number of points in the specified rectangle.
  int range(int i, int j, int a, int b) const {
    return range(i, j, a, b, 0, (1 << height)-1);
  }

  int range(int i, int j, int a, int b, int L, int U, int l=0) const {
    if (b < L || U < a)
        return 0;

    int M = L + (U-L)/2;
    if (a <= L && U <= b)
      return j - i + 1;
    else {
      int left = range(B[l].rank0(i-1), B[l].rank0(j) - 1,
                             a, b, L, M, l + 1);
      int right = range(z[l] + B[l].rank1(i-1), z[l] + B[l].rank1(j) - 1,
                              a, b, M+1, U, l+1);
      return left + right;
    }
  }
};
