/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * -----------------------------------------------------------------------------
 */
#include "wavelet-matrix.hpp"
#include "wavelet-tree.hpp"
#include "utils.hpp"
#include "bitmap.hpp"
#include <boost/timer/timer.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace boost::timer;

vector<int> sigmas = {1000,
                      10000,
                      100000,
                      1000000,
                      10000000,
                      100000000,
                      1000000000};

template<class Wavelet>
void testSize(istream &in, int persize, int sigma) {
  cpu_timer timer;
  int size = rawRead<int>(in);
  cout << "Sequence size - " << size << endl;
  double total = 0;
  vector<int> seq;
  for (int i = 0; i < persize; ++i) {
    rawRead(in, seq, size);

    timer.start();
    Wavelet wavelet(seq, sigma);
    timer.stop();

    auto millis = timer.elapsed().wall/1000000.0;
    total += millis;
  }

  cout << total << " " << total/persize << "\n\n";
}

int main() {
  ifstream in;
  int sigma = 1000;
  in.open("s"+to_string(sigma));
  sigma = rawRead<int>(in);

  int sizes = rawRead<int>(in);
  int persize = rawRead<int>(in);

  cout << persize << endl << endl;
  auto p = in.tellg();

  cout << "Wavelet Matrix\n";
  cout << "--------------" << endl;
  in.seekg(p);
  for (int i = 0; i < sizes; ++i)
    testSize<WaveMatrix<BitmapRankVec>>(in, persize, sigma);
  cout << endl;

  cout << "Wavelet Matrix with compression\n";
  cout << "--------------" << endl;
  in.seekg(p);
  for (int i = 0; i < sizes; ++i)
    testSize<WaveMatrix<BitmapRank>>(in, persize, sigma);

  cout << "Wavelet Tree\n";
  cout << "--------------" << endl;
  in.seekg(p);
  for (int i = 0; i < sizes; ++i)
    testSize<WaveTree<BitmapRankVec>>(in, persize, sigma);
  cout << endl;

  cout << "Wavelet Tree with compression\n";
  cout << "--------------" << endl;
  in.seekg(p);
  for (int i = 0; i < sizes; ++i)
    testSize<WaveTree<BitmapRank>>(in, persize, sigma);
  cout << endl;

  return 0;
}
