#include "jpeg_io.hpp"

#include <jpeglib.h>

#include <cstdio>
#include <stdexcept>
#include <vector>
#include <string>

// Simple RAII wrapper to ensure FILE handles are always closed
struct FileGuard {
    FILE* file;

    explicit FileGuard(FILE* f) : file(f) {}

    ~FileGuard() {
        if (file) {
            fclose(file);
        }
    }

    FILE* get() const {
        return file;
    }
};

namespace {

// Basic validation to avoid processing corrupted or inconsistent image buffers
void validateImageBuffer(const Image& image)
{
    const size_t expected =
        static_cast<size_t>(image.width) *
        static_cast<size_t>(image.height);

    if (image.width <= 0 || image.height <= 0) {
        throw std::runtime_error("Invalid image dimensions");
    }

    if (image.y.size() != expected ||
        image.cb.size() != expected ||
        image.cr.size() != expected)
    {
        throw std::runtime_error("Corrupted image buffer sizes");
    }
}

} // namespace

// Decode JPEG file into internal YCbCr-based Image structure
Image loadJPEG(const std::string& path)
{
    FileGuard file(fopen(path.c_str(), "rb"));

    if (!file.get()) {
        throw std::runtime_error("Failed to open input JPEG: " + path);
    }

    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file.get());

    jpeg_read_header(&cinfo, TRUE);

    // Force libjpeg to output in YCbCr format for consistent processing
    cinfo.out_color_space = JCS_YCbCr;

    jpeg_start_decompress(&cinfo);

    if (cinfo.output_components != 3) {
        jpeg_destroy_decompress(&cinfo);
        throw std::runtime_error("Unsupported JPEG format");
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

    // Temporary buffer for one scanline from libjpeg
    const int rowStride =
        image.width * cinfo.output_components;

    std::vector<uint8_t> rowBuffer(rowStride);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        JSAMPROW rowPtr[1];
        rowPtr[0] = rowBuffer.data();

        jpeg_read_scanlines(&cinfo, rowPtr, 1);

        const size_t row = cinfo.output_scanline - 1;

        for (int col = 0; col < image.width; ++col)
        {
            const size_t idx = row * image.width + col;
            const int base = col * 3;

            image.y[idx]  = rowBuffer[base];
            image.cb[idx] = rowBuffer[base + 1];
            image.cr[idx] = rowBuffer[base + 2];
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return image;
}

// Encode internal Image structure back into JPEG format
bool saveJPEG(const std::string& path,
              const Image& image)
{
    try {
        // Ensure input buffers are valid before encoding
        validateImageBuffer(image);

        FileGuard file(fopen(path.c_str(), "wb"));

        if (!file.get()) {
            return false;
        }

        jpeg_compress_struct cinfo;
        jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);

        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, file.get());

        cinfo.image_width  = image.width;
        cinfo.image_height = image.height;

        cinfo.input_components = 3;
        cinfo.in_color_space   = JCS_YCbCr;

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
                const size_t idx = row * image.width + col;
                const int base = col * 3;

                rowBuffer[base]     = image.y[idx];
                rowBuffer[base + 1] = image.cb[idx];
                rowBuffer[base + 2] = image.cr[idx];
            }

            JSAMPROW rowPtr[1];
            rowPtr[0] = rowBuffer.data();

            jpeg_write_scanlines(&cinfo, rowPtr, 1);
        }

        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);

        return true;
    }
    catch (...) {
        return false;
    }
}