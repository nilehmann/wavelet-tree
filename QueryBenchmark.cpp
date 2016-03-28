/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * -----------------------------------------------------------------------------
 */
#include <fstream>
#include "wavelet-matrix.hpp"
#include "wavelet-tree.hpp"
#include "bitmap.hpp"
#include <iostream>
#include "utils.hpp"
#include <boost/timer/timer.hpp>
using namespace boost::timer;
vector<double> sigmas = {1.0, .75, .5, .25};
vector<int> sizes = {100,
                     1000,
                     10000,
                     100000,
                     1000000};

void dummyfunction();

template<class Wavelet>
void runRankQuery(Wavelet &wave, tuple<int, int, int, int> query) {
  wave.rank(get<0>(query), get<1>(query));
}

template<class Wavelet>
void runRangeQuery(Wavelet &wave,
                   tuple<int, int, int, int> query) {
  wave.range(get<0>(query), get<1>(query), get<2>(query), get<3>(query));
}

template<class Wavelet>
void runQuantileQuery(Wavelet &wave, tuple<int, int, int, int> query) {
  wave.quantile(get<0>(query), get<1>(query), get<2>(query));
}

template<class Wavelet>
void runQueries(vector<int> seq, int sigma, int query_type,
                ostream &log,
                const vector<tuple<int, int, int, int>> &queries) {
  cpu_timer timer;
  Wavelet wave(seq, sigma);

  timer.start();
  for (auto query : queries)
    switch (query_type) {
    case 1: runRankQuery(wave, query); break;
    case 2: runQuantileQuery(wave, query); break;
    case 3: runRangeQuery(wave, query); break;
    }
  timer.stop();
  log << seq.size() << ";"
      << sigma << ";"
      << timer.elapsed().wall << ";"
      << timer.elapsed().wall/1000000.0 << ";"
      << queries.size() << ";"
      << 1.0*timer.elapsed().wall/queries.size() << ";"
      << "\n";
}


int main (int argc, char *argv[]) {
  if (argc < 3) {
    cout << "Usage: " << argv[0] << " query_type structure\n";
    cout << "1: rank\n";
    cout << "2: quantile\n";
    cout << "3: range\n";
    cout << "1: Wavelet Tree\n";
    cout << "2: Wavelet Tree Compresed\n";
    cout << "3: Wavelet Matrix\n";
    cout << "4: Wavelet Matrix Compressed\n";
    exit(0);
  }
  int query_type = atoi(argv[1]);
  int istructure = atoi(argv[2]);
  string structure;
  switch (istructure) {
  case 1: structure = "wavelet-tree"; break;
  case 2: structure = "wavelet-tree-compressed"; break;
  case 3: structure = "wavelet-matrix"; break;
  case 4: structure = "wavelet-matrix-compressed"; break;
  }

  string hd = "Size;Sigma;Total time[ns];Total time[ms];nqueries;Avg time[ns]\n";

  ofstream log; log.open("log/"+structure);

  cout << "Testing " << structure << endl;

  for (auto s : sigmas) {
    for (auto size : sizes) {
      char buf[256];
      sprintf(buf, "%.2f", s);
      int sigma = size*s;

      ifstream in;
      in.open("s"+string(buf)+"n"+to_string(size));
      size = rawRead<int>(in); // Size
      sigma = rawRead<int>(in); // Sigma
      vector<int> seq;
      rawRead(in, seq, size);

      int nqueries = rawRead<int>(in);
      vector<tuple<int, int, int, int>> queries(nqueries);
      for (int i = 0; i < nqueries; ++i) {
        switch (query_type) {
        case 1: queries[i] = tuple_cat(rawRead<tuple<int, int>>(in),
                                      make_tuple(0, 0));
          break;
        case 2: queries[i] = tuple_cat(rawRead<tuple<int, int, int>>(in),
                                      make_tuple(0));
          break;
        case 3: queries[i] = tuple_cat(rawRead<tuple<int, int, int, int>>(in));
          break;
        default:
          cout << "Invalid query\n";
        }
      }

      cout << "Size: " << size << "\n";
      cout << "Sigma: " << sigma << "\n";
      switch (istructure) {
      case 1: runQueries<WaveTree<BitmapRankVec>>(seq, sigma, query_type, log, queries); break;
      case 2: runQueries<WaveTree<BitmapRank>>(seq, sigma, query_type, log, queries); break;
      case 3: runQueries<WaveMatrix<BitmapRankVec>>(seq, sigma, query_type, log, queries); break;
      case 4: runQueries<WaveMatrix<BitmapRank>>(seq, sigma, query_type, log, queries); break;
      }

      cout << endl;
    }
  }
  log.close();
}
