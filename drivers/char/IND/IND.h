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
#define DMA_HALT                 0x04
#define DMA_RESET                0x08
#define FPGA_RESET               0x10
#define ADC_TEST_DATA            0x20
#define PPS_DEBUG_MODE           0x40
#define DMA_DEBUG_MODE           0x80

#define MODE_NORMAL              0x00
#define MODE_DMA_DEBUG           (DMA_DEBUG_MODE)
#define MODE_TRIGGER_DMA         (DMA_DEBUG_MODE|DEBUG_START_DMA)
#define MODE_PPS_DEBUG           (PPS_DEBUG_MODE)
#define MODE_TRIGGER_PPS         (PPS_DEBUG_MODE|GENERATE_PPS)

#define DISABLE_INTERRUPT        0
#define ENABLE_INTERRUPT         1

/*
** Status constants
*/
#define BIT_SPI_BUSY             0x01
#define BIT_S2MM_ERR             0x02
#define BIT_MM2S_RD_CMPLT        0x04
#define BIT_MM2S_ERR             0x08
#define BIT_SPI_ERR              0x10
#define BIT_INTERRUPT_ACTIVE     0x20
#define BIT_FPGA_RESET_STATUS    0x40
#define BIT_ADC_TEST_STATUS      0x80
#define BIT_PPS_DEBUG_STATUS     0x100
#define BIT_DMA_RESET_STATUS     0x200
#define BIT_DMA_DEBUG_STATUS     0x400
#define BIT_INTERRUPT_EN_STATUS  0x800
#define BIT_BAT_LOW_STATUS       0x1000
#define BIT_AC_OK_STATUS         0x2000

#define DMA_LENGTH	(128*1024*1024)

#define IND_DEBUG_READ        1
#define IND_DEBUG_WRITE       2
#define IND_DEBUG_DMA_READ    3
#define IND_DEBUG_DMA_WRITE   4

/* LED definitions
 * */
#define LED_RUNNING           0x01
#define LED_ALERT             0x02
#define LED_SPARE             0x04
#define LED_PPS_OK            0x08
#define LED_3G_OK             0x10
#define LED_WS_OK             0x20

/* CTRL bit definitions
 * */
#define CTRL_RESET_3G         0x01
#define CTRL_POWER_3G         0x02
#define CTRL_EN_SELECT        0x04


enum IND_user_cmds
{
   IND_USER_RESET,
   IND_USER_DMA_RESET,
   IND_USER_SET_MODE,
   IND_USER_SET_ADDRESS,
   IND_USER_DMA_TEST,
   IND_USER_TRIG_PPS,
   IND_USER_SPI_WRITE,
   IND_USER_STATUS,
   IND_USER_SET_LEDS,
   IND_USER_CLEAR_LEDS,
   IND_USER_SET_CTRL,
   IND_USER_CLEAR_CTRL,
   IND_USER_GET_SEM,
   IND_USER_SET_SEM,
   IND_USER_REG_DEBUG
};

/*
 *  struct IND_registers.
 *  This structure points to the first block where the registers are located
 */

struct IND_cmd_struct {
   __u32                            config;
   __u32                            interrupt;
   __u32                            address;
   __u32                            capture_count;
   __u32                            delay_count;
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

#define IND_USER_RESET              _IOWR(IND_IOCTL_BASE, 0x80, struct IND_cmd_struct)
#define IND_USER_DMA_RESET          _IOWR(IND_IOCTL_BASE, 0x81, struct IND_cmd_struct)
#define IND_USER_SET_MODE           _IOWR(IND_IOCTL_BASE, 0x82, struct IND_cmd_struct)
#define IND_USER_SET_ADDRESS        _IOWR(IND_IOCTL_BASE, 0x83, struct IND_cmd_struct)
#define IND_USER_DMA_TEST           _IOWR(IND_IOCTL_BASE, 0x84, struct IND_cmd_struct)
#define IND_USER_TRIG_PPS           _IOWR(IND_IOCTL_BASE, 0x85, struct IND_cmd_struct)
#define IND_USER_SPI_WRITE          _IOWR(IND_IOCTL_BASE, 0x86, struct IND_cmd_struct)
#define IND_USER_STATUS             _IOWR(IND_IOCTL_BASE, 0x87, struct IND_cmd_struct)
#define IND_USER_SET_LEDS           _IOWR(IND_IOCTL_BASE, 0x88, struct IND_cmd_struct)
#define IND_USER_CLEAR_LEDS         _IOWR(IND_IOCTL_BASE, 0x89, struct IND_cmd_struct)
#define IND_USER_SET_CTRL           _IOWR(IND_IOCTL_BASE, 0x8a, struct IND_cmd_struct)
#define IND_USER_CLEAR_CTRL         _IOWR(IND_IOCTL_BASE, 0x8b, struct IND_cmd_struct)
#define IND_USER_GET_SEM            _IOWR(IND_IOCTL_BASE, 0x8c, struct IND_cmd_struct)
#define IND_USER_SET_SEM            _IOWR(IND_IOCTL_BASE, 0x8d, struct IND_cmd_struct)
#define IND_USER_REG_DEBUG          _IOWR(IND_IOCTL_BASE, 0x8e, struct IND_cmd_struct)

#endif /* _IND_H */
