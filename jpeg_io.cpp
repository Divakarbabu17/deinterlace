// jpeg_io.cpp

#include "jpeg_io.hpp"

#include <jpeglib.h>

#include <cstdio>
#include <stdexcept>
#include <vector>

namespace {

void validateImageBuffer(const Image& image)
{
    const size_t expectedSize =
        static_cast<size_t>(image.width) *
        static_cast<size_t>(image.height);

    if (image.y.size() != expectedSize ||
        image.cb.size() != expectedSize ||
        image.cr.size() != expectedSize)
    {
        throw std::runtime_error(
            "Invalid image channel buffer size");
    }
}

} // namespace

Image loadJPEG(const std::string& path)
{
    FILE* file = fopen(path.c_str(), "rb");

    if (!file) {
        throw std::runtime_error(
            "Failed to open input JPEG: " + path);
    }

    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, file);

    jpeg_read_header(&cinfo, TRUE);

    // Force YCbCr output
    cinfo.out_color_space = JCS_YCbCr;

    jpeg_start_decompress(&cinfo);

    if (cinfo.output_components != 3) {
        jpeg_destroy_decompress(&cinfo);
        fclose(file);

        throw std::runtime_error(
            "Unsupported JPEG format");
    }

    Image image;

    image.width  = static_cast<int>(cinfo.output_width);
    image.height = static_cast<int>(cinfo.output_height);

    const size_t pixelCount =
        static_cast<size_t>(image.width) *
        static_cast<size_t>(image.height);

    image.y.resize(pixelCount);
    image.cb.resize(pixelCount);
    image.cr.resize(pixelCount);

    const int rowStride =
        image.width * cinfo.output_components;

    std::vector<uint8_t> rowBuffer(rowStride);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        JSAMPROW rowPointer[1];
        rowPointer[0] = rowBuffer.data();

        jpeg_read_scanlines(&cinfo, rowPointer, 1);

        const size_t row =
            cinfo.output_scanline - 1;

        for (int col = 0; col < image.width; ++col)
        {
            const size_t pixelIndex =
                row * image.width + col;

            const int base = col * 3;

            image.y[pixelIndex] =
                rowBuffer[base];

            image.cb[pixelIndex] =
                rowBuffer[base + 1];

            image.cr[pixelIndex] =
                rowBuffer[base + 2];
        }
    }

    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(file);

    return image;
}

void saveJPEG(const std::string& path,
              const Image& image)
{
    validateImageBuffer(image);

    FILE* file = fopen(path.c_str(), "wb");

    if (!file) {
        throw std::runtime_error(
            "Failed to open output JPEG: " + path);
    }

    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, file);

    cinfo.image_width = image.width;
    cinfo.image_height = image.height;

    cinfo.input_components = 3;

    cinfo.in_color_space = JCS_YCbCr;

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, 95, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    const int rowStride =
        image.width * cinfo.input_components;

    std::vector<uint8_t> rowBuffer(rowStride);

    while (cinfo.next_scanline < cinfo.image_height)
    {
        const size_t row = cinfo.next_scanline;

        for (int col = 0; col < image.width; ++col)
        {
            const size_t pixelIndex =
                row * image.width + col;

            const int base = col * 3;

            rowBuffer[base] =
                image.y[pixelIndex];

            rowBuffer[base + 1] =
                image.cb[pixelIndex];

            rowBuffer[base + 2] =
                image.cr[pixelIndex];
        }

        JSAMPROW rowPointer[1];
        rowPointer[0] = rowBuffer.data();

        jpeg_write_scanlines(&cinfo,
                             rowPointer,
                             1);
    }

    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);

    fclose(file);
}