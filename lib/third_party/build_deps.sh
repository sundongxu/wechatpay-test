#!/bin/bash

# Build hiredis, a C/C++ Client library for redis >= 1.2
git clone https://github.com/redis/hiredis.git
cd hiredis
make -j
make install 

cd ..

# Build libbcrype, a C++ wrapper around Bcrypt password hashing algorithm
git clone https://github.com/sundongxu/libbcrypt
cd libbcrypt
mkdir build
cd build
cmake ..
make -j
sudo make install
sudo ldconfig
