#ifndef TAREFA1B_NUMERICAL_HPP
#define TAREFA1B_NUMERICAL_HPP

#include "matrix.hpp"
#include <cmath>
#include <cstddef>
#include <algorithm>

namespace tarefa1b {

// 2D convolution with zero-padding (border pixels = 0)
inline Matrix<double> convolve(const Matrix<double>& img, const Matrix<double>& kernel) {
    auto rows = img.rows();
    auto cols = img.cols();
    auto kr = kernel.rows();
    auto kc = kernel.cols();
    auto half_r = kr / 2;
    auto half_c = kc / 2;

    Matrix<double> result(rows, cols, 0.0);

    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            double sum = 0.0;
            for (std::size_t kr_i = 0; kr_i < kr; ++kr_i) {
                for (std::size_t kc_j = 0; kc_j < kc; ++kc_j) {
                    auto ir = r + kr_i;
                    auto ic = c + kc_j;
                    if (ir >= half_r && ic >= half_c) {
                        auto src_r = ir - half_r;
                        auto src_c = ic - half_c;
                        if (src_r < rows && src_c < cols) {
                            sum += img(src_r, src_c) * kernel(kr_i, kc_j);
                        }
                    }
                }
            }
            result(r, c) = sum;
        }
    }
    return result;
}

// Normalize matrix values to [0, 1]
inline Matrix<double> normalize(const Matrix<double>& m) {
    double mn = m.raw()[0], mx = m.raw()[0];
    for (auto v : m.raw()) {
        mn = std::min(mn, v);
        mx = std::max(mx, v);
    }
    double range = mx - mn;
    if (range < 1e-12) return Matrix<double>(m.rows(), m.cols(), 0.0);
    return m.apply([mn, range](double v) { return (v - mn) / range; });
}

} // namespace tarefa1b

#endif // TAREFA1B_NUMERICAL_HPP
