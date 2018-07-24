#!/bin/bash

sudo apt-get install zsh git gfortran zlib1g-dev cmake libjpeg-dev lib32z1 imagemagick liblapack-dev


git clone https://github.com/kekliu/bundler_sfm
git clone https://github.com/kekliu/CMVS-PMVS


BASE_PATH=$(dirname $(which $0))

cd $BASE_PATH
cd bundler_sfm
make

cd ../CMVS-PMVS/program
mkdir build
cd build
cmake . ..
make
