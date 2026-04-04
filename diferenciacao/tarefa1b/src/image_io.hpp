#ifndef TAREFA1B_IMAGE_IO_HPP
#define TAREFA1B_IMAGE_IO_HPP

#include "matrix.hpp"
#include <fstream>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <algorithm>

namespace tarefa1b {

// Minimal BMP reader/writer for 24-bit uncompressed BMPs.
// Converts to grayscale Matrix<double> in [0,1].

namespace detail {

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t type{0x4D42};
    uint32_t size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset{54};
};

struct BMPInfoHeader {
    uint32_t size{40};
    int32_t  width{0};
    int32_t  height{0};
    uint16_t planes{1};
    uint16_t bit_count{24};
    uint32_t compression{0};
    uint32_t image_size{0};
    int32_t  x_ppm{0};
    int32_t  y_ppm{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
};
#pragma pack(pop)

} // namespace detail

inline Matrix<double> load_bmp(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file: " + path);

    detail::BMPFileHeader fh;
    detail::BMPInfoHeader ih;
    file.read(reinterpret_cast<char*>(&fh), sizeof(fh));
    file.read(reinterpret_cast<char*>(&ih), sizeof(ih));

    if (fh.type != 0x4D42)
        throw std::runtime_error("Not a BMP file: " + path);
    if (ih.bit_count != 24 || ih.compression != 0)
        throw std::runtime_error("Only 24-bit uncompressed BMP supported");

    auto width  = static_cast<std::size_t>(std::abs(ih.width));
    auto height = static_cast<std::size_t>(std::abs(ih.height));
    bool top_down = ih.height < 0;

    std::size_t row_size = ((width * 3 + 3) / 4) * 4; // padded to 4 bytes

    file.seekg(fh.offset, std::ios::beg);
    std::vector<uint8_t> row_buf(row_size);

    Matrix<double> img(height, width);

    for (std::size_t y = 0; y < height; ++y) {
        file.read(reinterpret_cast<char*>(row_buf.data()), static_cast<std::streamsize>(row_size));
        std::size_t target_y = top_down ? y : (height - 1 - y);
        for (std::size_t x = 0; x < width; ++x) {
            uint8_t b = row_buf[x * 3 + 0];
            uint8_t g = row_buf[x * 3 + 1];
            uint8_t r = row_buf[x * 3 + 2];
            // Luminance
            double gray = (0.299 * r + 0.587 * g + 0.114 * b) / 255.0;
            img(target_y, x) = gray;
        }
    }
    return img;
}

inline void save_bmp(const std::string& path, const Matrix<double>& img) {
    auto width  = static_cast<int32_t>(img.cols());
    auto height = static_cast<int32_t>(img.rows());
    std::size_t row_size = ((img.cols() * 3 + 3) / 4) * 4;

    detail::BMPFileHeader fh;
    detail::BMPInfoHeader ih;
    ih.width  = width;
    ih.height = height; // bottom-up
    ih.image_size = static_cast<uint32_t>(row_size * img.rows());
    fh.size = 54 + ih.image_size;

    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot write file: " + path);

    file.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    file.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> row_buf(row_size, 0);

    for (std::size_t y = 0; y < img.rows(); ++y) {
        std::size_t src_y = img.rows() - 1 - y; // bottom-up
        for (std::size_t x = 0; x < img.cols(); ++x) {
            double v = std::max(0.0, std::min(1.0, img(src_y, x)));
            auto byte = static_cast<uint8_t>(v * 255.0 + 0.5);
            row_buf[x * 3 + 0] = byte;
            row_buf[x * 3 + 1] = byte;
            row_buf[x * 3 + 2] = byte;
        }
        file.write(reinterpret_cast<const char*>(row_buf.data()), static_cast<std::streamsize>(row_size));
    }
}

} // namespace tarefa1b

#endif // TAREFA1B_IMAGE_IO_HPP
