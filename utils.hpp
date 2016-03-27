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

void set_bit(int &v, int i, bool b=true) {
  v |= (b << i);
}
bool get_bit(int v, int i) {
  return v & (1 << i);
}
random_device rd;
mt19937 gen(rd());

int intervalLength(int a, int b) {
  return b - a + 1;
}

// Generate a subinterval of [0, n)
tuple<int, int> genInterval(int n) {
  uniform_int_distribution<> pos_dis(0, n - 1);

  int a = pos_dis(gen);
  int b = pos_dis(gen);
  if (a > b)
    swap(a, b);
  return make_tuple(a, b);
}

vector<int> genSequence(int size, int sigma) {
  uniform_int_distribution<> dis(0, sigma - 1);

  vector<int> vec(size);
  for (int i = 0; i < size; ++i)
    vec[i] = dis(gen);

  return vec;
}

vector<int> genSeqContiguous(int size, int sigma) {
  uniform_int_distribution<> dis(0, sigma - 1);

  vector<int> seq(size);
  for (int i = 0; i < sigma; ++i)
    seq[i] = i;
  for (int i = sigma; i < size; ++i)
    seq[i] = dis(gen);
  random_shuffle(seq.begin(), seq.end());
  return seq;
}

vector<int> genNormalSequence(int size, int mean) {
  normal_distribution<> dis(mean, mean/5.0);
  std::map<int, int> hist;
  int sum = 0;
  int sigma = 0;
  while (sum < size) {
    int n = round(dis(gen));
    if (n <= 0)
      continue;
    if (sum + n > size)
      n = size - sum;
    sum += n;
    hist[n]++;
    sigma++;
  }

  vector<int> nums(sigma);
  for (int i = 0; i < (int)nums.size(); ++i)
    nums[i] = i;
  std::random_shuffle(nums.begin(), nums.end());

  vector<int> seq(size);
  int i = 0;
  int l = 0;
  for (auto p : hist) {
    cout << p.first << " " << p.second << "\n";
    // How many numbers appears this many times
    for (int k = 0; k < p.second; k++) {
      // How many times the number appears
      for (int j = 0; j < p.first; j++)
        seq[l++] = nums[i];
      i++;
    }
  }
  random_shuffle(seq.begin(), seq.end());

  return seq;
}

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

ostream& operator<<(ostream& stream, tuple<int, int> t) {
  stream << get<0>(t) << " " << get<1>(t);
  return stream;
}

ostream& operator<<(ostream& stream, tuple<int, int, int> t) {
  stream << get<0>(t) << " " << get<1>(t) << " " << get<2>(t);
  return stream;
}

ostream& operator<<(ostream& stream, tuple<int, int, int, int> t) {
  stream << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << " " << get<3>(t);
  return stream;
}


ostream& operator<<(ostream& stream, const vector<int> &vec) {
  for (int i = 0; i < (int)vec.size(); ++i) {
    if (i > 0)
      stream << " ";
    stream << vec[i];
  }
  return stream;
}

#endif
