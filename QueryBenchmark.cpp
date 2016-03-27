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
void runRankQuery(Wavelet &wave,
                  const vector<tuple<int, int, int, int>> &queries,
                  vector<int> &res) {
  // int i = 0;
  for (auto query : queries) {
    // res[i++] = wave.rank(get<0>(query), get<1>(query));
    wave.rank(get<0>(query), get<1>(query));
  }
}
template<class Wavelet>
void runRangeQuery(Wavelet &wave,
                   const vector<tuple<int, int, int, int>> &queries,
                   vector<int> &res) {
  // int i = 0;
  for (auto query : queries) {
    // res[i++] = wave.range(get<0>(query), get<1>(query), get<2>(query), get<3>(query));
    wave.range(get<0>(query), get<1>(query), get<2>(query), get<3>(query));
  }
}

template<class Wavelet>
void runQuantileQuery(Wavelet &wave,
                      const vector<tuple<int, int, int, int>> &queries,
                      vector<int> &res) {
  for (auto query : queries) {
    wave.quantile(get<0>(query), get<1>(query), get<2>(query));
  }
}

template<class Wavelet>
void runQueries(vector<int> seq, int sigma, int query_type,
                const vector<tuple<int, int, int, int>> &queries,
                vector<int> &res) {
  cpu_timer timer;
  Wavelet wave(seq, sigma);

  res.resize(queries.size());

  timer.start();
  // for (int i = 0; i < 100; ++i)
    switch (query_type) {
    case 1: runRankQuery(wave, queries, res); break;
    case 2: runQuantileQuery(wave, queries, res); break;
    case 3: runRangeQuery(wave, queries, res); break;
    }
  timer.stop();
  // cout << 1.0*timer.elapsed().wall/queries.size() << "\n";
  cout << timer.elapsed().wall << "\n";
}


int main (int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " query_type\n";
    cout << "1: rank\n";
    cout << "2: quantile\n";
    cout << "3: range\n";
    exit(0);
  }
  int query_type = atoi(argv[1]);

  double s = 0.5;
  int size = 1000000;
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
  cout << size << " " << sigma << " " << nqueries << "\n";
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

  vector<int> res1;
  runQueries<WaveTree<BitmapRankVec>>(seq, sigma, query_type, queries, res1);
  vector<int> res2;
  runQueries<WaveMatrix<BitmapRankVec>>(seq, sigma, query_type, queries, res2);
  // runQueries<WTNode<BitmapRank>>(seq, sigma, query_type, queries, res1);

  // for (int i = 0; i < (int)res1.size(); ++i)
  //   if(res1[i] != res2[i])
  //     cout << res1[i] << " " << res2[i] << endl;
}
