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
#include "utils.hpp"
using namespace std;

vector<int> sizes = {100,
                     1000,
                     10000,
                     100000,
                     1000000};

vector<double> sigmas = {1, .75, .5, .25};

int nqueries = 1000000;


tuple<int, int> genRankQuery(int size, int sigma) {
  uniform_int_distribution<> dis(0, sigma - 1);
  uniform_int_distribution<> dis2(0, size - 1);

  int i = genUniform(0, size - 1);
  int c = genUniform(0, sigma - 1);

  return make_tuple(c, i);
}

tuple<int, int, int> genQuantileQuery(int size) {
  int i, j;
  tie(i, j) = genInterval(size);

  uniform_int_distribution<> dis(1, intervalLength(i, j));
  int k = genUniform(1, intervalLength(i,j));

  return make_tuple(k, i, j);
}

tuple<int, int, int, int>
genRangeQuery(int size, int sigma) {
  int i, j, a, b;
  tie(i, j) = genInterval(size);
  tie(a, b) = genInterval(sigma);

  return make_tuple(i, j, a, b);
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " query [nqueries]\n";
    cout << "1: rank\n";
    cout << "2: quantile\n";
    cout << "3: range\n";
    exit(0);
  }
  int query = atoi(argv[1]);
  if (argc >= 3)
    nqueries = atoi(argv[2]);

  for (double s : sigmas) {
    for (int size : sizes) {
      char buf[256];
      sprintf(buf, "%.2f", s);
      int sigma = s*size;
      ofstream out;
      out.open("s"+string(buf)+"n"+to_string(size));

      // Generate and print sequence
      auto seq = genSeqContiguous(size, sigma);
      rawPrint(out, (int)seq.size());
      rawPrint(out, sigma);
      rawPrint(out, seq);

      // Generate and print queries
      rawPrint(out, nqueries);
      for (int i = 0; i < nqueries; ++i)
        switch (query) {
        case 1: rawPrint(out,genRankQuery(size, sigma)); break;
        case 2: rawPrint(out,genQuantileQuery(size)); break;
        case 3: rawPrint(out,genRangeQuery(size, sigma)); break;
        default: cout << "Invalid query type\n";
        }
      out.close();
    }
  }


}
