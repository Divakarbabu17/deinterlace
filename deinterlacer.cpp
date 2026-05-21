#include "deinterlacer.hpp"

void Deinterlacer::processChannel(const std::vector<uint8_t>& in,
                                  std::vector<uint8_t>& out,
                                  int width,
                                  int height)
{
    // Row 0 will be keeping as it is 
    for (int c = 0; c < width; c++) {
        out[c] = in[c];
    }

    for (int r = 1; r < height; r++) {
        for (int c = 0; c < width; c++) {

            int cur = r * width + c;
            int prev = (r - 1) * width + c;

            out[cur] =
                (in[cur] + in[prev]) / 2;
        }
    }
}

Image Deinterlacer::process(const Image& input)
{
    Image out = input;

    processChannel(input.y,  out.y,  input.width, input.height);
    processChannel(input.cb, out.cb, input.width, input.height);
    processChannel(input.cr, out.cr, input.width, input.height);

    return out;
}