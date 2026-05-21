Deinterlacer (JPEG YCbCr Processing Tool)

This project is a command-line application that applies a simple deinterlacing (line blending) algorithm on JPEG images using the YCbCr color space.

JPEG input → decode → processing → encode JPEG output.

---------------------------------------------------------------------
Features

JPEG decoding/encoding using libjpeg

Processing in YCbCr color space

Dockerized execution environment

CMake build support

Unit test for core algorithm validation

-------------------------------------------------------------------------

Build and test Instructions

## I. Docker (I Recommend this)

This project is fully containerized.

##  Docker (Recommended)

The recommended way to build and run the application is using Docker.

### Build Image

```bash
docker build -t deinterlacer-app .
```

### Run Application

```bash
docker run -it -v ${PWD}:/app deinterlacer-app input.jpg output.jpg
```

This will generate the processed output JPEG image.



## II. Alternative Local Build Methods

These build methods are optional and intended only for local development or debugging purposes.

### Option 1 — CMake (Recommended for local builds)

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Option 2 — Manual g++ Build

```bash
g++ -std=c++17 main.cpp deinterlacer.cpp jpeg_io.cpp -o app -ljpeg
```


--------------------------------------------------------------------------------
Sample Input / Output

A sample image is provided to validate the implementation.

ls sample/

input.jpg  output.jpg  output.txt

root@c66fa5bc29a3:/app#

--------------------------------------------------------------------------------
Unit Tests

A lightweight unit test validates the core  logic.



 
