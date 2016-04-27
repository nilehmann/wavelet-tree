#ifndef UTILS_HPP
#define UTILS_HPP

#define log2(x) (sizeof(uint)*8 - __builtin_clz(x))

#define popcnt(x) __builtin_popcount(x)

#define set_bit(v, i, b) v |= ((b) << (i))
#define get_bit(v, i) ((v) & (1 << (i)))

#endif
