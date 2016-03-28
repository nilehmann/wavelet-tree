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

using namespace boost::timer;

vector<double> sigmas = {1, .75, .5, .25};

template<class Wavelet>
void testSize(istream &in, ostream &summary, int persize) {
  // ofstream log;
  cpu_timer timer;
  int size = rawRead<int>(in);
  int sigma = rawRead<int>(in);
  cout << "Sequence size: " << size << endl;
  cout << "Sigma: " << sigma << endl;

  vector<int> seq;

  // log.open("logs/s"+to_string(sigma)+"n"+to_string(size));
  // log << "Time [ms]; Memory [MB]\n";

  double total_time = 0;
  double total_mem = 0;
  for (int i = 0; i < persize; ++i) {
    rawRead(in, seq, size);

    timer.start();
    Wavelet wavelet(seq, sigma);
    timer.stop();

    auto millis = timer.elapsed().wall/1000000.0;
    double mem = wavelet.memory()/1024.0/1024;

    total_mem += mem;
    total_time += millis;

    // log << millis << ";" << mem << "\n";
  }
  summary << sigma << ";"
          << size << ";"
          << persize << ";"
          << total_time << ";"
          << total_time/persize << ";"
          << 1.0*total_mem/persize/1024/1024 << endl;

  cout << "Total time [ms]: " << total_time << "\n"
       << "Average time [ms]: " << total_time/persize << "\n\n";

  // log.close();
}

int main() {
  ifstream in;
  ofstream summary_wmc, summary_wm, summary_wt, summary_wtc;
  summary_wm.open("wavelet-matrix");
  summary_wm << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[MB]\n";
  summary_wmc.open("wavelet-matrix-compression");
  summary_wmc << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[MB]\n";
  summary_wt.open("wavelet-tree");
  summary_wt << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[MB]\n";
  summary_wtc.open("wavelet-tree-compression");
  summary_wtc << "Sigma;Size;Number;Total time[ms];Avg Time[ms];Avg Mem[MB]\n";

  for (auto s : sigmas) {
    char buf[256];
    sprintf(buf, "%.2f", s);
    in.open("s"+string(buf));


    int sizes = rawRead<int>(in);
    int persize = rawRead<int>(in);

    cout << persize << endl << endl;
    auto p = in.tellg();


    cout << "Wavelet Tree\n";
    cout << "------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveTree<BitmapRankVec>>(in, summary_wt, persize);
    cout << endl;


    cout << "Wavelet Matrix\n";
    cout << "--------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveMatrix<BitmapRankVec>>(in, summary_wm, persize);
    cout << endl;


    cout << "Wavelet Tree with compression\n";
    cout << "-----------------------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveTree<BitmapRank>>(in, summary_wtc, persize);
    cout << endl;


    cout << "Wavelet Matrix with compression\n";
    cout << "-------------------------------" << endl;
    in.seekg(p);
    for (int i = 0; i < sizes; ++i)
      testSize<WaveMatrix<BitmapRank>>(in, summary_wmc, persize);
    cout << endl;



    in.close();
  }

  summary_wm.close();
  summary_wmc.close();
  summary_wt.close();
  summary_wtc.close();

  return 0;

}
