#!/bin/bash

wget https://github.com/pdtechvn/toolschain/raw/master/arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
tar -xvf arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz

echo [Build kernel...]
ARCH_TYPE=arm
CROSS_COMPILE_TYPE=`pwd`/4.9.3/bin/arm-none-linux-gnueabihf-
make -j8 ARCH=$ARCH_TYPE CROSS_COMPILE=$CROSS_COMPILE_TYPE jf4418_linux_defconfig
make -j8 ARCH=$ARCH_TYPE CROSS_COMPILE=$CROSS_COMPILE_TYPE uImage

