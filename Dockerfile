FROM ubuntu:22.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    libjpeg-dev \
    git \
    gdb

WORKDIR /app