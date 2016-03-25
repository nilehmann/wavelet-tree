/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * -----------------------------------------------------------------------------
 */
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
#include "wavelet-matrix.hpp"
#include "wavelet-tree.hpp"
#include "bitmap.hpp"
using namespace std;

vector<int> readSeq(ifstream &in, int &sigma) {
  int size;
  in >> size >> sigma;

  vector<int> vec(size);
  for (int i = 0; i < size; ++i)
    in >> vec[i];
  return vec;
}

vector<tuple<int, int>> readRankQueries(ifstream &in) {
  int queries;
  in >> queries;
  vector<tuple<int, int>> vec(queries);
  for (int q = 0; q < queries; ++q) {
    int c, i;
    in >> c >> i;
    vec[q] = make_tuple(c, i);
  }
  in.close();
  return vec;
}

vector<tuple<int, int, int>> readQuantileQueries(ifstream &in) {
  int queries;
  in >> queries;
  vector<tuple<int, int, int>> vec(queries);
  for (int q = 0; q < queries; ++q) {
    int k, i, j;
    in >> k >> i >> j;
    vec[q] = make_tuple(k, i, j);
  }
  in.close();
  return vec;
}

vector<tuple<int, int, int, int>> readRangeQueries(ifstream &in) {
  int queries;
  in >> queries;
  vector<tuple<int, int, int, int>> vec(queries);
  for (int q = 0; q < queries; ++q) {
    int i, j, a, b;
    in >>  i >> j >> a >> b;
    vec[q] = make_tuple(i, j, a, b);
  }
  in.close();
  return vec;
}


int main() {
  ifstream in_seq, in_quantile, in_range, in_rank;
  in_seq.open("seq-010000000-000");
  in_quantile.open("quantile-010000000-000");
  in_range.open("range-010000000-000");
  in_rank.open("rank-010000000-000");

  int sigma;
  vector<int> vec = readSeq(in_seq, sigma);

  in_seq.close();


  WaveTree<BitmapRankVec> tree(vec, sigma);
  // WaveletMatrix<BitmapRank> tree(vec);
  // for (auto query : readRankQueries(in_rank)) {
  //   int c = get<0>(query);
  //   int a = get<1>(query);
  //   int res = get<2>(query);
  //   assert(tree.rank(c, a) == res);
  // }
  for (auto query : readQuantileQueries(in_quantile)) {
    int k = get<0>(query);
    int a = get<1>(query);
    int b = get<2>(query);
    int res = get<3>(query);
    assert(tree.quantile(k, a, b) == res);
  }
  for (auto query : readRangeQueries(in_range)) {
    int i = get<0>(query);
    int j = get<1>(query);
    int a = get<2>(query);
    int b = get<3>(query);
    int res = get<4>(query);
    assert(tree.range(i, j, a, b) == res);
  }

  return 0;
}
