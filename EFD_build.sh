#!/bin/bash
#
# SEPL Linux build (sepl_linux_build.sh)
#

# Specify the build directory.
BUILD_DIR="../build/sepl_linux/zynq_IND"

MAKE_FLAGS="CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm O=${BUILD_DIR}"


# Remove output directory (for a complete new build).
rm -rf ${BUILD_DIR}

# Make .config file for zynq_IND target.
make ${MAKE_FLAGS} zynq_IND_defconfig

# Make the kernel.
make ${MAKE_FLAGS}

# Make u-boot wrapped kernel image.
make ${MAKE_FLAGS} uImage LOADADDR=0x00008000

# Make the IND device tree.
make ${MAKE_FLAGS} arch/arm/boot/dts/zynq-IND.dtb

