#include<vector>
#include<algorithm>
#include <iostream>
using namespace std;
typedef vector<int>::iterator iter;

template<class Bitmap>
class WaveTree {
  vector<Bitmap> r0; int n, s;
  // vector<vector<int>> r0; int n, s;

  void build(iter b, iter e, int l, int r, int u) {
    if (l == r)
      return;
    int m = (l+r)/2;
    // r0[u].reserve(e-b+1); r0[u].push_back(0);
    // for (iter it = b; it != e; ++it)
    //     r0[u].push_back(r0[u].back() + (*it<=m));

    r0[u].resize(e-b+1);
    int i; iter it;
    for (it = b, i=0; it !=e; ++it, ++i)
      r0[u].set(i, *it<=m);
    r0[u].build_rank();

    iter p = stable_partition(b, e, [=](int i){
        return i<=m;});

    build(b, p, l, m, u*2);
    build(p, e, m+1, r, u*2+1);
  }

  int q, w;
  int range(int i, int j, int l, int r, int u) {
    if (r < q or w < l)
      return 0;
    if (q <= l and r <= w)
      return j-i;
    // int m = (l+r)/2, ri = r0[u][i], rj = r0[u][j];
    int m = (l+r)/2, ri = r0[u].rank1(i-1), rj = r0[u].rank1(j-1);
    return range(ri, rj, l, m, u*2) +
      range(i-ri, j-rj, m+1, r, u*2+1);
  }

public:
  //arr[i] in [0,sigma)
  WaveTree(vector<int> &arr, int sigma) {
    n = arr.size(); s = sigma;
    r0.resize(s*2);
    build(arr.begin(), arr.end(), 0, s-1, 1);
  }

  //k in [1,n], [i,j] is 0-indexed, -1 if error
  int quantile(int k, int i, int j) {
    j++;
    if (k < 1 or k > j-i)
      return -1;
    int l = 0, r = s-1, u = 1, m, ri, rj;
    while (l != r) {
      m = (l+r)/2;
      // ri = r0[u][i]; rj = r0[u][j]; u*=2;
      ri = r0[u].rank1(i-1); rj = r0[u].rank1(j-1); u*=2;
      if (k <= rj-ri)
        i = ri, j = rj, r = m;
      else
        k -= rj-ri, i -= ri, j -= rj,
          l = m+1, ++u;
    }
    return r;
  }

  //counts numbers in [a,b] in positions [i,j]
  int range(int i, int j, int a, int b) {
    if (b < a or j < i)
      return 0;
    q = a; w = b;
    return range(i, j+1, 0, s-1, 1);
  }

  //count occurrences of x in positions [0,i]
  int rank(int x, int i) {
    i++;
    int l = 0, r = s-1, u = 1, m, z;
    while (l != r) {
      m = (l+r)/2;
      // z = r0[u][i]; u*=2;
      z = r0[u].rank1(i-1); u*=2;
      if (x <= m)
        i = z, r = m;
      else
        i -= z, l = m+1, ++u;
    }
    return i;
  }

  // //x in [0,sigma)
  // void push_back(int x) {
  //   int l = 0, r = s-1, u = 1, m, p; ++n;
  //   while (l != r) {
  //     m = (l+r)/2;
  //     p = (x<=m);
  //     r0[u].push_back(r0[u].back() + p);
  //     u*=2;
  //     if (p) r = m; else l = m+1, ++u;
  //   }
  // }

  //doesn't check if empty
  // void pop_back() {
  //   int l = 0, r = s-1, u = 1, m, p, k; --n;
  //   while (l != r) {
  //     m = (l+r)/2; k = r0[u].size();
  //     p = r0[u][k-1] - r0[u][k-2];
  //     r0[u].pop_back();
  //     u*=2;
  //     if (p) r = m; else l = m+1, ++u;
  //   }
  // }
};
