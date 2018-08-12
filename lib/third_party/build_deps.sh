#!/bin/bash
# Build Dependencies
echo '------------------------Dependencies Installation Begin------------------------'
# 1. grpc
echo '-------------------------------GRPC Installation-------------------------------'
sudo apt-get install build-essential autoconf libtool pkg-config
sudo apt-get install libgflags-dev libgtest-dev
sudo apt-get install clang libc++-dev
git clone https://github.com/grpc/grpc.git
cd grpc
git submodule update --init
make -j
sudo make install 
echo '-----------------------------Installation Finished-----------------------------'
cd ..

# 2. hiredis, a C/C++ Client library for redis >= 1.2
echo '------------------------------Hiredis Installtion------------------------------'
git clone https://github.com/redis/hiredis.git
cd hiredis
make -j
sudo make install 
echo '-----------------------------Installation Finished-----------------------------'
cd ..

# 3. libbcrypt, a C++ wrapper around Bcrypt password hashing algorithm
echo '-----------------------------Libbrypt Installation-----------------------------'
git clone https://github.com/sundongxu/libbcrypt
cd libbcrypt
mkdir build
cd build
cmake ..
make -j
sudo make install
sudo ldconfig
echo '-----------------------------Installation Finished-----------------------------'
echo '--------------------------All Dependencies Installed---------------------------'

