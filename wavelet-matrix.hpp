/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * -----------------------------------------------------------------------------
 */
#ifndef WAVELET_MATRIX_HPP
#define WAVELET_MATRIX_HPP
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

  WaveMatrix(vector<int> &S);

  WaveMatrix(vector<int> &S, int sigma);

  int memory();

  // Find the k-th smallest element. The smallest element is k=1
  int b = 0 ;
  int quantile(int k, int i, int j);

  // Count occurrences of number c until position i
  int rank(int c, int i);
  // Count number of occurrences of numbers in the range [a, b]
  // present in the sequence at range [i, j], ie, if representing a grid it
  // counts number of points in the specified rectangle.
  int range(int i, int j, int a, int b);

  int range(int i, int j, int a, int b, int L, int U, int l=0);
};

#endif
