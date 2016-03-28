#ifndef WAVELET_TREE_HPP
#define WAVELET_TREE_HPP
#include<vector>
#include<algorithm>
#include <iostream>
using namespace std;
typedef vector<int>::iterator iter;

template<class Bitmap>
class WaveTree {
  vector<Bitmap> r0; int n, s;
  mutable int q, w;
private:
  void build(iter b, iter e, int l, int r, int u);
  int range(int i, int j, int l, int r, int u);
public:
  int memory();
  //arr[i] in [0,sigma)
  WaveTree(vector<int> &arr, int sigma);

  //k in [1,n], [i,j] is 0-indexed, -1 if error
  int quantile(int k, int i, int j);

  //counts numbers in [a,b] in positions [i,j]
  int range(int i, int j, int a, int b);

  //count occurrences of x in positions [0,i]
  int rank(int x, int i);
};

#endif
