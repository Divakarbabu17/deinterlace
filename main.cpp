#include <iostream>
#include <fstream>

#include "deinterlacer.hpp"
#include "jpeg_io.hpp"

void writeChannel(std::ofstream& out,
                  const std::vector<uint8_t>& ch,
                  int w,
                  int h,
                  const std::string& name)
{
    out << "\n" << name << "\n";

    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            out << (int)ch[r * w + c] << " ";
        }
        out << "\n";
    }
}

void writeImage(std::ofstream& out,
                const Image& img,
                const std::string& title)
{
    out << "\n====================\n";
    out << title << "\n";
    out << "====================\n";

    writeChannel(out, img.y,  img.width, img.height, "Y");
    writeChannel(out, img.cb, img.width, img.height, "Cb");
    writeChannel(out, img.cr, img.width, img.height, "Cr");
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: ./app input.jpg output.jpg\n";
        return 1;
    }

    const std::string inputPath  = argv[1];
    const std::string outputPath = argv[2];

    try {

        // Load JPEG
        Image img = loadJPEG(inputPath);

        std::ofstream debugFile("output.txt");

        writeImage(debugFile,
                   img,
                   "INPUT IMAGE");

        // Process image
        Deinterlacer d;

        Image out = d.process(img);

        writeImage(debugFile,
                   out,
                   "OUTPUT IMAGE");

        debugFile.close();

        // Save processed JPEG
        saveJPEG(outputPath, out);

        std::cout << "Deinterlacing completed successfully.\n";
    }
    catch (const std::exception& ex) {

        std::cerr << "Error: "
                  << ex.what()
                  << "\n";

        return 1;
    }

    return 0;
}