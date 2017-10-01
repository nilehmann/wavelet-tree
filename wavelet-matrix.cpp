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
#include "bitmap.hpp"
using namespace std;

typedef unsigned int uint;

// Wavelet Matrix with succinct representation of bitmaps
struct WaveMatrixSucc {
  uint height;
  vector<BitmapRank> B;
  vector<int> z;

  WaveMatrixSucc(vector<int> &A) :
    WaveMatrixSucc(A, *max_element(A.begin(), A.end()) + 1) {}

  // sigma = size of the alphabet, ie., one more than the maximum element
  // in A.
  WaveMatrixSucc(vector<int> &A, int sigma)
    : height(log2(sigma - 1)),
      B(height), z(height) {
    for (uint l = 0; l < height; ++l) {
      B[l].resize(A.size());
      for (uint i = 0; i < A.size(); ++i)
        B[l].set(i, get_bit(A[i], height - l - 1));
      B[l].build_rank();

      auto it = stable_partition(A.begin(), A.end(), [=] (int c) {
          return not get_bit(c, height - l - 1);
        });
      z[l] = distance(A.begin(), it);
    }
  }

  // Count occurrences of number c until position i.
  // ie, occurrences of c in positions [0,i]
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

  // Find the k-th smallest element in positions [i,j].
  // The smallest element is k=1
  int quantile(int k, int i, int j) const {
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
        set_bit(element, height - l - 1, 1);
      }
    }
    return element;
  }

  // Count number of occurrences of numbers in the range [a, b]
  // present in the sequence in positions [i, j], ie, if representing a grid it
  // counts number of points in the specified rectangle.
  int range(int i, int j, int a, int b) const {
    return range(i, j, a, b, 0, (1 << height)-1, 0);
  }

  int range(int i, int j, int a, int b, int L, int U, int l) const {
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
