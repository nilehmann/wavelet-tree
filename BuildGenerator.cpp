/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * -----------------------------------------------------------------------------
 */
#include "utils.hpp"
#include <fstream>
#include <iostream>

vector<int> sizes = {100,
                     1000,
                     10000,
                     100000,
                     1000000};
vector<double> sigmas = {1, .75, .5, .25};

int persize = 1000;

int main() {
  for (auto s : sigmas) {
    ofstream out;
    char buf[256];
    sprintf(buf, "%.2f", s);
    out.open("s"+string(buf));
    rawPrint(out, (int)sizes.size());
    rawPrint(out, persize);

    for (int size : sizes) {
      int sigma = s*size;
      cout << "Generating sequences with s="
           << sigma << " and n=" << size << endl;

      rawPrint(out, size);
      rawPrint(out, sigma);
      for (int i = 0; i < persize; ++i) {
        auto seq = genSeqContiguous(size, sigma);
        rawPrint(out, seq);
      }
    }
    out.close();
  }

  return 0;
}
