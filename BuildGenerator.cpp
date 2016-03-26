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
                     // 10000000};
vector<int> sigmas = {1000,
                      10000,
                      100000,
                      1000000};
                      // 10000000,
                      // 100000000,
                      // 1000000000};

int persize = 1000;

int main(int argc, char *argv[]) {
  vector<int> seq = genNormalSequence(100, 20);
  cout << seq << "\n";


  // for (int sigma : sigmas) {
  //   ofstream out;
  //   out.open("s"+to_string(sigma));
  //   // out << sigma << endl;
  //   rawPrint(out, sigma);
  //   // out << sizes.size() << " " << persize << endl;
  //   rawPrint(out, (int)sizes.size());
  //   rawPrint(out, persize);

  //   for (int size : sizes) {
  //     cout << "Generating sequences with s=" << sigma << " and n=" << size << endl;
  //     // out << size << endl;
  //     rawPrint(out, size);
  //     for (int i = 0; i < persize; ++i) {
  //       auto seq = genSequence(size, sigma);
  //       // out << seq << "\n";
  //       rawPrint(out, seq);
  //     }
  //   }
  //   out.close();
  // }

  return 0;
}
