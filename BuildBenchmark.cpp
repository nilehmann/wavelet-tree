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
                      1000000};

template<class Wavelet>
void testSize(istream &in, ostream &summary, int persize, int sigma) {
  ofstream log;
  cpu_timer timer;
  int size = rawRead<int>(in);
  cout << "Sequence size: " << size << endl;

  vector<int> seq;

  log.open("s"+to_string(sigma)+"n"+to_string(size));
  log << "Time [ms]; Memory [bytes]\n";

  double total_time = 0;
  int total_mem = 0;
  for (int i = 0; i < persize; ++i) {
    rawRead(in, seq, size);

    timer.start();
    Wavelet wavelet(seq, sigma);
    timer.stop();

    auto millis = timer.elapsed().wall/1000000.0;
    int mem = wavelet.memory();

    total_mem += mem;
    total_time += millis;

    log << millis << ";" << mem << "\n";
  }
  summary << sigma << ";"
          << size << ";"
          << persize << ";"
          << total_time << ";"
          << total_time/persize << ";"
          << 1.0*total_mem/persize << endl;

  cout << "Total time [ms]: " << total_time << "\n"
       << "Average time [ms]: " << total_time/persize << "\n\n";

  log.close();
}

int main() {
  ifstream in;
  ofstream summary_wmc, summary_wm, summary_wt, summary_wtc;
  summary_wm.open("wavelet-matrix");
  summary_wm << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[bytes]\n";
  summary_wmc.open("wavelet-matrix-compression");
  summary_wmc << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[bytes]\n";
  summary_wt.open("wavelet-tree");
  summary_wt << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[bytes]\n";
  summary_wtc.open("wavelet-tree-compression");
  summary_wtc << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[bytes]\n";

  for (int sigma : sigmas) {
    in.open("s"+to_string(sigma));
    rawRead<int>(in);

    int sizes = rawRead<int>(in);
    int persize = rawRead<int>(in);

    cout << persize << endl << endl;
    auto p = in.tellg();

    cout << "Wavelet Matrix\n";
    cout << "--------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveMatrix<BitmapRankVec>>(in, summary_wm, persize, sigma);
    cout << endl;



    cout << "Wavelet Matrix with compression\n";
    cout << "-------------------------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveMatrix<BitmapRank>>(in, summary_wmc, persize, sigma);
    cout << endl;



    cout << "Wavelet Tree\n";
    cout << "------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveTree<BitmapRankVec>>(in, summary_wt, persize, sigma);
    cout << endl;



    cout << "Wavelet Tree with compression\n";
    cout << "-----------------------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveTree<BitmapRank>>(in, summary_wtc, persize, sigma);
    cout << endl;

    in.close();
  }

  summary_wm.close();
  summary_wmc.close();
  summary_wt.close();
  summary_wtc.close();

  return 0;
}
