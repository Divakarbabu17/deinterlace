Deinterlacer (JPEG YCbCr Processing Tool)

This project is a command-line application that applies a simple deinterlacing (line blending) algorithm on JPEG images using the YCbCr color space.

The goal of the implementation is to process images in a pipeline style:
JPEG input → decode → processing → encode JPEG output.


-------------------------------------------------------------------------

Build Instructions

Build the project using g++ with libjpeg:
g++ -std=c++17 main.cpp deinterlacer.cpp jpeg_io.cpp -o app -ljpeg

--------------------------------------------------------------------------------

Run Instructions

./app input.jpg output.jpg

--------------------------------------------------------------------------------
Sample Input / Output

A sample image is provided to validate the implementation.
ls sample/
input.jpg  output.jpg  output.txt
root@c66fa5bc29a3:/app#





 
