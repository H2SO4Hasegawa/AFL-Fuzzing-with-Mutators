#!/bin/bash
cd ~

# install packages
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update

sudo apt install -y cmake
sudo apt-get install -y m4
sudo apt install -y ninja-build
sudo apt install -y fdupes
sudo apt install -y python3
sudo apt install -y python3-pip

sudo apt-get -y install autoconf 
sudo apt-get install -qq -yy libgmp-dev 
sudo apt-get -y install libgnomecanvas2-dev
sudo apt-get -y install graphviz
sudo apt-get install -qq -yy libgtksourceview2.0-dev
sudo apt-get -y install libexporter-lite-perl libfile-which-perl libgetopt-tabular-perl   
sudo apt-get -y install libregexp-common-perl flex build-essential zlib1g-dev
sudo apt-get -y install libterm-readkey-perl

## install compilers
sudo apt-get -y install gcc-11 g++-11 cpp-11
sudo apt-get -y install gcc-10 g++-10 cpp-10

## set gcov
sudo apt-get -y install clang-10
sudo apt-get -y install clang-format-10
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-10 100 --slave /usr/bin/clang++ clag++ /usr/bin/clang++-10
sudo rm /usr/bin/cpp /usr/bin/gcc /usr/bin/g++  /usr/bin/gcov  /usr/bin/c++
sudo ln -s /usr/bin/cpp-10 /usr/bin/cpp
sudo ln -s /usr/bin/gcc-10 /usr/bin/gcc
sudo ln -s /usr/bin/g++-10 /usr/bin/g++
sudo ln -s /usr/bin/g++-10 /usr/bin/c++
sudo ln -s /usr/bin/gcov-10 /usr/bin/gcov

# For Cmake
sudo apt-get install libssl-dev
