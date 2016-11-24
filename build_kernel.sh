#!/bin/bash

if [ -f "uboot-mkimage" ]; then
echo "uboot-mkimage folder exist"
else
git clone https://github.com/pdtechvn/uboot-mkimage.git
fi

cd uboot-mkimage
make
export PATH=$PATH:$PWD
cd ../

if [ -f "arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz" ]; then
echo "toolchains exist"
else
wget https://github.com/pdtechvn/toolschain/raw/master/arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
tar -xvf arm-cortexa9-linux-gnueabihf-4.9.3.tar.xz
fi

echo [Build kernel...]
ARCH_TYPE=arm
CROSS_COMPILE_TYPE=`pwd`/4.9.3/bin/arm-none-linux-gnueabihf-
make -j2 ARCH=$ARCH_TYPE CROSS_COMPILE=$CROSS_COMPILE_TYPE jf4418_linux_defconfig
make -j2 ARCH=$ARCH_TYPE CROSS_COMPILE=$CROSS_COMPILE_TYPE uImage
make -j2 ARCH=$ARCH_TYPE CROSS_COMPILE=$CROSS_COMPILE_TYPE modules
