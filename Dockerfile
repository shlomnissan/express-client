FROM ubuntu:22.04

ENV DEBIAN_FRONTEND='noninteractive'

RUN apt-get update && \
    apt-get install --no-install-recommends -y \
    build-essential \
    git-all \
    cmake \
    ninja-build \
    clang \
    libc++-dev \
    lldb \
    libc++abi-dev \
    clangd \
    clang-tidy \
    clang-format \
    libssl-dev \
    python3-pip