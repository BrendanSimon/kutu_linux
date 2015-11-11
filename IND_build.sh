source /opt/Xilinx/Vivado/2015.3/settings64.sh
source /opt/Xilinx/SDK/2015.3/settings64.sh
export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
export ARCH=arm
make zynq_IND_defconfig
make uImage LOADADDR=0x00008000
