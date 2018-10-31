#!/bin/bash

#
# ORIGINAL contents from Greg Smart.
#
#source /opt/Xilinx/Vivado/2015.3/settings64.sh
#source /opt/Xilinx/SDK/2015.3/settings64.sh
#export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
#export ARCH=arm
#make zynq_IND_defconfig
#make uImage LOADADDR=0x00008000


#
# Brendan's build.
#
# make mrproper
# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND mrproper

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND zynq_IND_defconfig

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND menuconfig

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND oldconfig

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND uImage LOADADDR=0x00008000

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND modules

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND dtbs

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND arch/arm/boot/dts/zynq-IND.dtb

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND KBUILD_DEBARCH=armhf deb-pkg

# make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND && make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND uImage LOADADDR=0x00008000 && make CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=../build/sepl_linux/zynq_IND KBUILD_DEBARCH=armhf deb-pkg

MAKE_FLAGS=""
MAKE_FLAGS="${MAKE_FLAGS} CROSS_COMPILE=arm-linux-gnueabihf-"
MAKE_FLAGS="${MAKE_FLAGS} ARCH=arm"
MAKE_FLAGS="${MAKE_FLAGS} O=../build/sepl_linux/zynq_IND"

# Make .config from zynq_IND_defconfig file provided by Greg Smart.
make ${MAKE_FLAGS} zynq_IND_defconfig

# Make the kernel.
make ${MAKE_FLAGS}

# Make the kernel modules.
make ${MAKE_FLAGS} modules

# Make u-boot wrapped kernel image.
make ${MAKE_FLAGS} uImage LOADADDR=0x00008000

# Make device trees (in arch/arm/boot/dts/Makefile).
make ${MAKE_FLAGS} dtbs

# Make specific device trees.
#make ${MAKE_FLAGS} arch/arm/boot/dts/zynq-IND.dtb
#make ${MAKE_FLAGS} arch/arm/boot/dts/zynq-zed-adv7511-ad9467-fmc-250ebz.dtb
#make ${MAKE_FLAGS} arch/arm/boot/dts/zynq-zed-adv7511-fmcomms1.dtb
#make ${MAKE_FLAGS} arch/arm/boot/dts/zynq-zed.dtb

# Make Debian package.
make ${MAKE_FLAGS} KBUILD_DEBARCH=armhf deb-pkg

# Install the kernel modules and kernel.
#sudo make ${MAKE_FLAGS} modules_install
#sudo make ${MAKE_FLAGS} install

