/*
 *  IND.h -- Register definitions for IND implementation
 *
 *  Greg Smart
 *
 *  Version 0.1 10/03/14
 *
 */

#ifndef _IND_H
#define _IND_H


/*
** configuration constants
*/
#define GENERATE_PPS             0x01
#define DEBUG_START_DMA          0x02
#define FPGA_RESET               0x10
#define ADC_TEST_DATA            0x20
#define PPS_DEBUG_MODE           0x40
#define DMA_DEBUG_MODE           0x80

#define MODE_NORMAL              0x00
#define MODE_DMA_DEBUG           (DMA_DEBUG_MODE)
#define MODE_TRIGGER_DMA         (DMA_DEBUG_MODE|DEBUG_START_DMA)
#define MODE_PPS_DEBUG           (PPS_DEBUG_MODE)
#define MODE_TRIGGER_PPS         (PPS_DEBUG_MODE|GENERATE_PPS)

/*
** SPI constants
*/
#define SPI_PORT_LOW             0x01
#define SPI_PORT_HIGH            0x02
#define SPI_PORT_BOTH            0x03


#define DMA_LENGTH	(64*1024*1024)

#define IND_DEBUG_READ        1
#define IND_DEBUG_WRITE       2
#define IND_DEBUG_DMA_READ    3
#define IND_DEBUG_DMA_WRITE   4

enum IND_user_cmds
{
   IND_USER_RESET,
   IND_USER_SET_CLK,
   IND_USER_SET_MODE,
   IND_USER_RUN_SCAN,
   IND_USER_DMA_TEST,
   IND_USER_TRIG_PPS,
   IND_USER_SPI_WRITE,
   IND_USER_STATUS,
   IND_USER_REG_DEBUG
};

/*
 *  struct IND_registers.
 *  This structure points to the first block where the registers are located
 */

struct IND_cmd_struct {
   __u32                            config;
   __u32                            address;
} ;

struct IND_spi_cmd_struct {
   __u32                           port_device[16];
   __u32                           port_addr[16];
   __u32                           port_data[16];
   __u32                           num_spi_writes;
} ;

struct IND_debug_struct {
   __u32                           cmd;
   __u32                           reg;
   __u32                           data;
} ;

#define IND_IOCTL_BASE	't'

#define IND_USER_RESET              _IOWR(IND_IOCTL_BASE, 0x81, struct IND_cmd_struct)
#define IND_USER_SET_CLK            _IOWR(IND_IOCTL_BASE, 0x82, struct IND_cmd_struct)
#define IND_USER_SET_MODE           _IOWR(IND_IOCTL_BASE, 0x83, struct IND_cmd_struct)
#define IND_USER_RUN_SCAN           _IOWR(IND_IOCTL_BASE, 0x84, struct IND_cmd_struct)
#define IND_USER_DMA_TEST           _IOWR(IND_IOCTL_BASE, 0x85, struct IND_cmd_struct)
#define IND_USER_TRIG_PPS           _IOWR(IND_IOCTL_BASE, 0x86, struct IND_cmd_struct)
#define IND_USER_SPI_WRITE          _IOWR(IND_IOCTL_BASE, 0x87, struct IND_cmd_struct)
#define IND_USER_STATUS             _IOWR(IND_IOCTL_BASE, 0x88, struct IND_cmd_struct)
#define IND_USER_REG_DEBUG          _IOWR(IND_IOCTL_BASE, 0x89, struct IND_cmd_struct)

#endif /* _IND_H */
