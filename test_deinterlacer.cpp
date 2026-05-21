#include <iostream>
#include <vector>
#include "deinterlacer.hpp"

void assertEqual(int a, int b, const std::string& msg)
{
    if (a != b) {
        std::cerr << "[FAIL] " << msg << "\n";
        exit(1);
    }
}

int main()
{
    Image img;
    img.width = 3;
    img.height = 2;

    // input:
    // row0: 10 10 10
    // row1: 20 20 20
    img.y = {
        10,10,10,
        20,20,20
    };

    img.cb = img.y;
    img.cr = img.y;

    Deinterlacer d;
    Image out = d.process(img);

    // row0 unchanged
    assertEqual(out.y[0], 10, "row0 col0");
    assertEqual(out.y[1], 10, "row0 col1");

    // row1 = avg(20,10) = 15
    assertEqual(out.y[3], 15, "row1 col0");
    assertEqual(out.y[4], 15, "row1 col1");

    std::cout << "[PASS] Deinterlacer tests passed\n";
}