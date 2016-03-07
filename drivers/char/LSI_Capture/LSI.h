/*
 *  LSI_Capture.h -- Register definitions for LSI_Capture implementation
 *
 *  Greg Smart
 *
 *  Version 0.1 10/03/14
 *
 */

#ifndef _LSI_Capture_H
#define _LSI_Capture_H

#define LSI_VERSION_MAJOR 0
#define LSI_VERSION_MINOR 3

/*
** configuration constants
*/
#define GENERATE_PPS             0x01
#define START_DMA                0x02
#define DMA_HALT                 0x04
#define DMA_RESET                0x08
#define FPGA_RESET               0x10
#define ADC_TEST_DATA            0x20
#define PPS_DEBUG_MODE           0x40
#define DMA_DEBUG_MODE           0x80
#define DEBUG_SELECT_CH0         0x000
#define DEBUG_SELECT_CH1         0x100
#define DEBUG_SELECT_CH2         0x200
#define DEBUG_SELECT_CH_OFF      0x300
#define DEBUG_SELECT_ACTIVE      0x800
#define SIGNED_DATA              0x1000

#define CONFIG_MODE_MASK         (START_DMA | ADC_TEST_DATA | PPS_DEBUG_MODE | DMA_DEBUG_MODE | DEBUG_SELECT_CH_OFF | DEBUG_SELECT_ACTIVE | SIGNED_DATA)


#define MODE_NORMAL              0x00
#define MODE_DMA_DEBUG           (DMA_DEBUG_MODE)
#define MODE_TRIGGER_DMA         (DMA_DEBUG_MODE|DEBUG_START_DMA)
#define MODE_PPS_DEBUG           (PPS_DEBUG_MODE)
#define MODE_TRIGGER_PPS         (PPS_DEBUG_MODE|GENERATE_PPS)
#define MODE_SYSTEM_HALT         (PPS_DEBUG_MODE)

#define MODE_CH_AUTO             0x00
#define MODE_CH_AUTO_INV         ~(DEBUG_SELECT_ACTIVE|DEBUG_SELECT_CH_OFF)
#define MODE_CH_0                (DEBUG_SELECT_ACTIVE|DEBUG_SELECT_CH0)
#define MODE_CH_1                (DEBUG_SELECT_ACTIVE|DEBUG_SELECT_CH1)
#define MODE_CH_2                (DEBUG_SELECT_ACTIVE|DEBUG_SELECT_CH2)
#define MODE_CH_OFF              (DEBUG_SELECT_ACTIVE|DEBUG_SELECT_CH_OFF)

#define MODE_SIGNED              (SIGNED_DATA)

#define PEAK_START_DISABLE       0x00ffffff
#define PEAK_STOP_DISABLE        0x00ffffff


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

#define DMA_LENGTH	(256*1024*1024)

#define LSI_DEBUG_READ        1
#define LSI_DEBUG_WRITE       2
#define LSI_DEBUG_DMA_READ    3
#define LSI_DEBUG_DMA_WRITE   4

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

/* SPI definitions
*/
#define SPI_CTRL_WRITE        0x0
#define SPI_CTRL_READ         0x00800000
#define SPI_DEVICE_0          0x0
#define SPI_DEVICE_1          0x01000000




enum LSI_Capture_user_cmds
{
   LSI_USER_RESET,
   LSI_USER_DMA_RESET,
   LSI_USER_SET_MODE,
   LSI_USER_SET_ADDRESS,
   LSI_USER_DMA_TEST,
   LSI_USER_TRIG_PPS,
   LSI_USER_SPI_ACCESS,
   LSI_USER_STATUS,
   LSI_USER_SET_LEDS,       /* use LSI_USER_MODIFY_LEDS to set and clear */
   LSI_USER_CLEAR_LEDS,     /* use LSI_USER_MODIFY_LEDS to set and clear */
   LSI_USER_SET_CTRL,       /* use LSI_USER_MODIFY_CTRL to set and clear */
   LSI_USER_CLEAR_CTRL,     /* use LSI_USER_MODIFY_CTRL to set and clear */
   LSI_USER_SET_INTERRUPT,
   LSI_USER_GET_SEM,
   LSI_USER_SET_SEM,
   LSI_USER_REG_DEBUG,
   LSI_USER_MODIFY_LEDS,
   LSI_USER_MODIFY_CTRL,
   LSI_USER_READ_MAXMIN,
   LSI_USER_INIT_LMK03000,
   LSI_USER_WRITE_TAPS,
   LSI_USER_READ_TAPS,
   LSI_USER_VERSION,
   LSI_USER_FPGA_VERSION
};

/*
 *  struct LSI_registers.
 *  This structure points to the first block where the registers are located
 */

struct LSI_cmd_struct {
   __u32                            config;
   __u32                            interrupt;
   __u32                            address;
   __u32                            capture_count;
   __u32                            delay_count;
   __u32                            peak_detect_start;
   __u32                            peak_detect_end;
} ;

/*
** Structure to set and clear bits for the following ioctl commands.
**   LSI_USER_MODIFY_LEDS
**   LSI_USER_MODIFY_CTRL
*/
typedef struct LSI_bit_flag_struct {
   __u32                            set;
   __u32                            clear;
} LSI_bit_flag_t;


struct LSI_spi_cmd_struct {
   __u32                           port_device[16];
   __u32                           port_addr[16];
   __u32                           port_data[16];
   __u32                           num_spi_writes;
   __u32                           num_spi_reads;
} ;

struct LSI_adc_tap_struct {
   __u32                           clk_taps;
   __u32                           frame_taps;
   __u32                           adc0_0_taps;
   __u32                           adc0_1_taps;
   __u32                           adc1_0_taps;
   __u32                           adc1_1_taps;
   __u32                           adc2_0_taps;
   __u32                           adc2_1_taps;
   __u32                           adc3_0_taps;
   __u32                           adc3_1_taps;
   __u32                           device;
   __u32                           frame_status;
} ;

struct LSI_debug_struct {
   __u32                           cmd;
   __u32                           reg;
   __u32                           data;
} ;

struct LSI_maxmin_struct {
   __u32                           max_ch0_data;
   __u32                           max_ch0_addr;
   __u32                           min_ch0_data;
   __u32                           min_ch0_addr;
   __u32                           max_ch1_data;
   __u32                           max_ch1_addr;
   __u32                           min_ch1_data;
   __u32                           min_ch1_addr;
   __u32                           max_ch2_data;
   __u32                           max_ch2_addr;
   __u32                           min_ch2_data;
   __u32                           min_ch2_addr;
} ;


#define LSI_IOCTL_BASE	't'

#define LSI_USER_RESET              _IOWR(LSI_IOCTL_BASE, 0x80, struct LSI_cmd_struct)
#define LSI_USER_DMA_RESET          _IOWR(LSI_IOCTL_BASE, 0x81, struct LSI_cmd_struct)
#define LSI_USER_SET_MODE           _IOWR(LSI_IOCTL_BASE, 0x82, struct LSI_cmd_struct)
#define LSI_USER_SET_ADDRESS        _IOWR(LSI_IOCTL_BASE, 0x83, struct LSI_cmd_struct)
#define LSI_USER_DMA_TEST           _IOWR(LSI_IOCTL_BASE, 0x84, struct LSI_cmd_struct)
#define LSI_USER_TRIG_PPS           _IOWR(LSI_IOCTL_BASE, 0x85, struct LSI_cmd_struct)
#define LSI_USER_SPI_ACCESS         _IOWR(LSI_IOCTL_BASE, 0x86, struct LSI_cmd_struct)
#define LSI_USER_STATUS             _IOWR(LSI_IOCTL_BASE, 0x87, struct LSI_cmd_struct)
#define LSI_USER_SET_LEDS           _IOWR(LSI_IOCTL_BASE, 0x88, struct LSI_cmd_struct)
#define LSI_USER_CLEAR_LEDS         _IOWR(LSI_IOCTL_BASE, 0x89, struct LSI_cmd_struct)
#define LSI_USER_SET_CTRL           _IOWR(LSI_IOCTL_BASE, 0x8a, struct LSI_cmd_struct)
#define LSI_USER_CLEAR_CTRL         _IOWR(LSI_IOCTL_BASE, 0x8b, struct LSI_cmd_struct)
#define LSI_USER_SET_INTERRUPT      _IOWR(LSI_IOCTL_BASE, 0x8c, struct LSI_cmd_struct)
#define LSI_USER_GET_SEM            _IOWR(LSI_IOCTL_BASE, 0x8d, struct LSI_cmd_struct)
#define LSI_USER_SET_SEM            _IOWR(LSI_IOCTL_BASE, 0x8e, struct LSI_cmd_struct)
#define LSI_USER_REG_DEBUG          _IOWR(LSI_IOCTL_BASE, 0x8f, struct LSI_cmd_struct)
#define LSI_USER_MODIFY_LEDS        _IOWR(LSI_IOCTL_BASE, 0x90, struct LSI_cmd_struct)
#define LSI_USER_MODIFY_CTRL        _IOWR(LSI_IOCTL_BASE, 0x91, struct LSI_cmd_struct)
#define LSI_USER_READ_MAXMIN        _IOWR(LSI_IOCTL_BASE, 0x92, struct LSI_maxmin_struct)
#define LSI_USER_INIT_LMK03000      _IOWR(LSI_IOCTL_BASE, 0x93, struct LSI_cmd_struct)
#define LSI_USER_WRITE_TAPS         _IOWR(LSI_IOCTL_BASE, 0x94, struct LSI_adc_tap_struct)
#define LSI_USER_READ_TAPS          _IOWR(LSI_IOCTL_BASE, 0x95, struct LSI_adc_tap_struct)
#define LSI_USER_VERSION            _IOWR(LSI_IOCTL_BASE, 0x96, struct LSI_cmd_struct)
#define LSI_USER_FPGA_VERSION       _IOWR(LSI_IOCTL_BASE, 0x97, struct LSI_cmd_struct)

#endif /* _LSI_H */
