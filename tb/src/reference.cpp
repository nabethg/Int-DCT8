#include "reference.h"
#include <cmath>
#include <iostream>

// normalization factor: α(0) = 1/√2, α(k)=1 for k>0
static inline double alpha(int k) {
    return (k == 0) ? (1.0 / std::sqrt(2.0)) : 1.0;
}

// C_k = α(k) * sum_{n=0}^{N-1} x_n * cos[ (π / N) * (n + 1/2) * k ],
// for k in [0, N-1]
void ref_dct_slow(const int8_t x[N], int32_t C[N]) {
    constexpr int N = 8;
    for (int k = 0; k < N; ++k) {
        double sum = 0.0;
        for (int n = 0; n < N; ++n) {
            double theta = M_PI * (n + 0.5) * k / N;
            sum += x[n] * std::cos(theta);
        }
        double value = sum * alpha(k);
        C[k] = static_cast<int32_t>(std::floor(value * 64)); // Q6 integer
    }
}
