#ifndef TAREFA1B_FILTERS_HPP
#define TAREFA1B_FILTERS_HPP

#include "matrix.hpp"
#include "numerical.hpp"
#include <cmath>

namespace tarefa1b {

// Gaussian kernel (size x size, sigma)
inline Matrix<double> gaussian_kernel(std::size_t size, double sigma) {
    Matrix<double> k(size, size);
    auto half = static_cast<int>(size / 2);
    double sum = 0.0;
    for (int r = -half; r <= half; ++r) {
        for (int c = -half; c <= half; ++c) {
            double val = std::exp(-(r * r + c * c) / (2.0 * sigma * sigma));
            k(static_cast<std::size_t>(r + half), static_cast<std::size_t>(c + half)) = val;
            sum += val;
        }
    }
    // Normalize
    for (std::size_t r = 0; r < size; ++r)
        for (std::size_t c = 0; c < size; ++c)
            k(r, c) /= sum;
    return k;
}

// Gaussian blur
inline Matrix<double> gaussian_blur(const Matrix<double>& img,
                                     std::size_t size = 5,
                                     double sigma = 1.4) {
    auto kernel = gaussian_kernel(size, sigma);
    return convolve(img, kernel);
}

// Sobel X kernel
inline Matrix<double> sobel_x_kernel() {
    Matrix<double> k(3, 3);
    k(0,0) = -1; k(0,1) = 0; k(0,2) = 1;
    k(1,0) = -2; k(1,1) = 0; k(1,2) = 2;
    k(2,0) = -1; k(2,1) = 0; k(2,2) = 1;
    return k;
}

// Sobel Y kernel
inline Matrix<double> sobel_y_kernel() {
    Matrix<double> k(3, 3);
    k(0,0) = -1; k(0,1) = -2; k(0,2) = -1;
    k(1,0) =  0; k(1,1) =  0; k(1,2) =  0;
    k(2,0) =  1; k(2,1) =  2; k(2,2) =  1;
    return k;
}

// Laplacian kernel
inline Matrix<double> laplacian_kernel() {
    Matrix<double> k(3, 3, 0.0);
    k(0,1) =  1;
    k(1,0) =  1; k(1,1) = -4; k(1,2) = 1;
    k(2,1) =  1;
    return k;
}

// Algoritmo 1: Sobel-based edge detection
// Returns binary image: edges = 0 (black), non-edges = 1 (white)
inline Matrix<double> algoritmo1(const Matrix<double>& img, double threshold) {
    // 1. Gaussian blur
    auto smoothed = gaussian_blur(img);

    // 2. Sobel gradients
    auto A = convolve(smoothed, sobel_x_kernel()); // dx
    auto B = convolve(smoothed, sobel_y_kernel()); // dy

    // 2.3 Square
    auto A2 = A.apply([](double v) { return v * v; });
    auto B2 = B.apply([](double v) { return v * v; });

    // 2.4 sqrt(A^2 + B^2)
    auto sum = A2 + B2;
    auto C = sum.apply([](double v) { return std::sqrt(v); });

    // Normalize gradient magnitude to [0,1] for thresholding
    auto Cn = normalize(C);

    // 3-4. Threshold: edge pixels = 0 (black), others = 1 (white)
    auto D = Cn.apply([threshold](double v) {
        return v >= threshold ? 0.0 : 1.0;
    });
    return D;
}

// Algoritmo 2: Laplacian-based edge detection
// Returns binary image: edges = 0 (black), non-edges = 1 (white)
inline Matrix<double> algoritmo2(const Matrix<double>& img, double tolerance = 0.0001) {
    // 1. Gaussian blur
    auto smoothed = gaussian_blur(img);

    // 2. Laplacian
    auto A = convolve(smoothed, laplacian_kernel());

    // 3. Binary: if |A| > tolerance => edge (0/black), else non-edge (1/white)
    auto B = A.apply([tolerance](double v) {
        return std::abs(v) > tolerance ? 0.0 : 1.0;
    });
    return B;
}

} // namespace tarefa1b

#endif // TAREFA1B_FILTERS_HPP
