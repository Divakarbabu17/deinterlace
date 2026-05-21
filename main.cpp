#include <iostream>
#include <fstream>
#include "deinterlacer.hpp"

void writeChannel(std::ofstream& out,
                  const std::vector<uint8_t>& ch,
                  int w, int h,
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

void writeImage(std::ofstream& out , Image& img, const std::string& title)
{

    out << "====================\n";
    out << title << "\n";
    out << "====================\n";

    writeChannel(out, img.y,  img.width, img.height, "Y");
    writeChannel(out, img.cb, img.width, img.height, "Cb");
    writeChannel(out, img.cr, img.width, img.height, "Cr");

}

int main()
{
    Image img;
    img.width = 3;
    img.height = 3;

    img.y  = {10,20,30, 40,50,60, 70,80,90};
    img.cb = img.y;
    img.cr = img.y;
    std::ofstream file("output.txt");
    writeImage(file, img, "INPUT IMAGE");

    Deinterlacer d;
    Image out = d.process(img);
    
    
    writeImage(file ,out, "OUTPUT IMAGE");

    std::cout << "data is  written to output.txt\n";

    return 0;
}