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
vector<int> sigmas = {1000,
                      10000,
                      100000,
                      1000000};
vector<int> sizes = {100,
                     1000,
                     10000,
                     100000,
                     1000000};

template<class Wavelet>
void runRankQuery(tuple<int, int> query) {

}


int main (int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " query_type\n";
    cout << "1: rank\n";
    cout << "2: quantile\n";
    cout << "3: range\n";
  }
  int query_type = atoi(argv[1]);

  int sigma = 1000;
  int size = 100;

  ifstream in;
  in.open("s"+to_string(sigma)+"n"+to_string(size));
  rawRead<int>(in); // Size
  rawRead<int>(in); // Sigma
  vector<int> seq;
  rawRead(in, seq, size);

  int nqueries = rawRead<int>(in);
  for (int i = 0; i < nqueries; ++i) {
    switch (query_type) {
    case 1: auto query = rawRead<tuple<int, int>>(in); break;
    }
}
