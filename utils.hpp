#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

#define log2(x) (sizeof(uint)*8 - __builtin_clz(x))

#define popcnt(x) __builtin_popcount(x)

#define set_bit(v, i, b) v |= ((b) << (i))
#define get_bit(v, i) ((v) & (1 << (i)))

int intervalLength(int a, int b);

// Generate a subinterval of [0, n)
tuple<int, int> genInterval(int n);

int genUniform(int a, int b);

vector<int> genSequence(int size, int sigma);

vector<int> genSeqContiguous(int size, int sigma);

vector<int> genNormalSequence(int size, int mean);

template<class T>
void rawPrint(ostream &stream, T a) {
  stream.write(reinterpret_cast<char*>(&a), sizeof(a));
}

template<class T>
void rawPrint(ostream &stream, vector<T> vec) {
  for (T a : vec)
    rawPrint(stream, a);
}

template<class T>
T rawRead(istream &stream) {
  T buf;
  stream.read(reinterpret_cast<char*>(&buf), sizeof(T));
  return buf;
}

template<class T>
void rawRead(istream &stream, vector<T> &vec, int n) {
  vec.resize(n);
  for (int i = 0; i < (int)vec.size(); i++)
    vec[i] = rawRead<T>(stream);
}


#endif
