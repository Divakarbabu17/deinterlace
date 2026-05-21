#ifndef DEINTERLACER_HPP
#define DEINTERLACER_HPP

#include "image.hpp"

class Deinterlacer {
public:
    Image process(const Image& input);

private:
    void processChannel(const std::vector<uint8_t>& in,
                        std::vector<uint8_t>& out,
                        int width,
                        int height);
};

#endif