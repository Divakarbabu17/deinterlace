#ifndef JPEG_IO_HPP
#define JPEG_IO_HPP

#include <string>
#include "image.hpp"

Image loadJPEG(const std::string& path);

bool saveJPEG(const std::string& path,
              const Image& image);

#endif