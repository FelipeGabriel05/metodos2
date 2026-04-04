#ifndef TAREFA1B_MATRIX_HPP
#define TAREFA1B_MATRIX_HPP

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <functional>
#include <cmath>

namespace tarefa1b {

template <typename T>
class Matrix {
public:
    Matrix() : rows_(0), cols_(0) {}

    Matrix(std::size_t rows, std::size_t cols, T val = T{})
        : rows_(rows), cols_(cols), data_(rows * cols, val) {}

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    T& operator()(std::size_t r, std::size_t c) {
        return data_.at(r * cols_ + c);
    }

    const T& operator()(std::size_t r, std::size_t c) const {
        return data_.at(r * cols_ + c);
    }

    // Element-wise operations
    Matrix<T> operator+(const Matrix<T>& o) const {
        if (rows_ != o.rows_ || cols_ != o.cols_)
            throw std::invalid_argument("Matrix dimensions mismatch");
        Matrix<T> result(rows_, cols_);
        for (std::size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] + o.data_[i];
        return result;
    }

    // Apply a function to every element
    Matrix<T> apply(std::function<T(T)> fn) const {
        Matrix<T> result(rows_, cols_);
        for (std::size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = fn(data_[i]);
        return result;
    }

    const std::vector<T>& raw() const { return data_; }
    std::vector<T>& raw() { return data_; }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<T> data_;
};

} // namespace tarefa1b

#endif // TAREFA1B_MATRIX_HPP
