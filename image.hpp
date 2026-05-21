#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <cstdint>

struct Image {
    int width = 0;
    int height = 0;

    std::vector<uint8_t> y;
    std::vector<uint8_t> cb;
    std::vector<uint8_t> cr;
};

#endif