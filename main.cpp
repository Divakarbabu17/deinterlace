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


class App
{
public:
    App(std::string input, std::string output)
        : inputPath(std::move(input)),
          outputPath(std::move(output)) {}

    void run()
    {
        Image input = loadJPEG(inputPath);

        validate(input);

        std::ofstream debugFile("output.txt");
        if (!debugFile.is_open()) {
            throw std::runtime_error("Cannot create debug file");
        }

        writeImage(debugFile, input, "INPUT IMAGE");

        Deinterlacer deinterlacer;
        Image output = deinterlacer.process(input);

        writeImage(debugFile, output, "OUTPUT IMAGE");

        debugFile.close();

        if (!saveJPEG(outputPath, output)) {
            throw std::runtime_error("Failed to save output JPEG");
        }
    }

private:
    std::string inputPath;
    std::string outputPath;

    void validate(const Image& img)
    {
        if (img.width <= 0 ||
            img.height <= 0 ||
            img.y.empty())
        {
            throw std::runtime_error("Invalid image loaded");
        }
    }
};


int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: ./app input.jpg output.jpg\n";
        return 1;
    }

    try {
        App app(argv[1], argv[2]);
        app.run();

        std::cout << "[INFO] Processing completed successfully\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] " << ex.what() << "\n";
        return 1;
    }

    return 0;
}