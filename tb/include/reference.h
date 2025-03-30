#ifndef REFERENCE_H
#define REFERENCE_H

#include <cmath>
#include <cstdint>

// sample count
constexpr int N = 8;

// iteratively compute the N-point DCT of the input sequence x[n]
void ref_dct_slow(const int8_t x[N], int32_t C[N]);

#endif
