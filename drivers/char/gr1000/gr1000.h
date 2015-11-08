/*
 *  gr1000.h -- Register definitions for GR1000 implementation
 *
 *  Greg Smart
 *
 *  Version 0.1 10/03/14
 *
 */

#ifndef _GR1000_H
#define _GR1000_H


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

#define GR1000_DEBUG_READ        1
#define GR1000_DEBUG_WRITE       2
#define GR1000_DEBUG_DMA_READ    3
#define GR1000_DEBUG_DMA_WRITE   4

enum gr1000_user_cmds
{
   GR1000_USER_RESET,
   GR1000_USER_SET_CLK,
   GR1000_USER_SET_MODE,
   GR1000_USER_RUN_SCAN,
   GR1000_USER_DMA_TEST,
   GR1000_USER_TRIG_PPS,
   GR1000_USER_SPI_WRITE,
   GR1000_USER_STATUS,
   GR1000_USER_REG_DEBUG
};

/*
 *  struct GR1000_registers.
 *  This structure points to the first block where the registers are located
 */

struct GR1000_cmd_struct {
   __u32                            config;
   __u32                            address;
} ;

struct GR1000_spi_cmd_struct {
   __u32                           port_device[16];
   __u32                           port_addr[16];
   __u32                           port_data[16];
   __u32                           num_spi_writes;
} ;

struct GR1000_debug_struct {
   __u32                           cmd;
   __u32                           reg;
   __u32                           data;
} ;

#define GR1000_IOCTL_BASE	't'

#define GR1000_USER_RESET              _IOWR(GR1000_IOCTL_BASE, 0x81, struct GR1000_cmd_struct)
#define GR1000_USER_SET_CLK            _IOWR(GR1000_IOCTL_BASE, 0x82, struct GR1000_cmd_struct)
#define GR1000_USER_SET_MODE           _IOWR(GR1000_IOCTL_BASE, 0x83, struct GR1000_cmd_struct)
#define GR1000_USER_RUN_SCAN           _IOWR(GR1000_IOCTL_BASE, 0x84, struct GR1000_cmd_struct)
#define GR1000_USER_DMA_TEST           _IOWR(GR1000_IOCTL_BASE, 0x85, struct GR1000_cmd_struct)
#define GR1000_USER_TRIG_PPS           _IOWR(GR1000_IOCTL_BASE, 0x86, struct GR1000_cmd_struct)
#define GR1000_USER_SPI_WRITE          _IOWR(GR1000_IOCTL_BASE, 0x87, struct GR1000_cmd_struct)
#define GR1000_USER_STATUS             _IOWR(GR1000_IOCTL_BASE, 0x88, struct GR1000_cmd_struct)
#define GR1000_USER_REG_DEBUG          _IOWR(GR1000_IOCTL_BASE, 0x89, struct GR1000_cmd_struct)

#endif /* _GR1000_H */
