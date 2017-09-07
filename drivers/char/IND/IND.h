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
#define GENERATE_PPS             (0x0001)
#define DEBUG_START_DMA          (0x0002)
#define DMA_HALT                 (0x0004)
#define DMA_RESET                (0x0008)
#define FPGA_RESET               (0x0010)
#define ADC_TEST_DATA            (0x0020)
#define PPS_DEBUG_MODE           (0x0040)
#define DMA_DEBUG_MODE           (0x0080)
#define DEBUG_SELECT_CH0         (0x0000)
#define DEBUG_SELECT_CH1         (0x0100)
#define DEBUG_SELECT_CH2         (0x0200)
#define DEBUG_SELECT_CH_OFF      (0x0300)
#define DEBUG_SELECT_ACTIVE      (0x0800)
#define SIGNED_DATA              (0x1000)

#define CONFIG_MODE_MASK         (ADC_TEST_DATA | PPS_DEBUG_MODE | DMA_DEBUG_MODE | DEBUG_SELECT_CH_OFF | DEBUG_SELECT_ACTIVE | SIGNED_DATA)


#define MODE_NORMAL              (0x00)
#define MODE_DMA_DEBUG           (DMA_DEBUG_MODE)
#define MODE_TRIGGER_DMA         (DMA_DEBUG_MODE | DEBUG_START_DMA)
#define MODE_PPS_DEBUG           (PPS_DEBUG_MODE)
#define MODE_TRIGGER_PPS         (PPS_DEBUG_MODE | GENERATE_PPS)
#define MODE_SYSTEM_HALT         (PPS_DEBUG_MODE)

#define MODE_CH_AUTO             (0x00)
#define MODE_CH_AUTO_INV         (~(DEBUG_SELECT_ACTIVE | DEBUG_SELECT_CH_OFF))
#define MODE_CH_0                (DEBUG_SELECT_ACTIVE | DEBUG_SELECT_CH0)
#define MODE_CH_1                (DEBUG_SELECT_ACTIVE | DEBUG_SELECT_CH1)
#define MODE_CH_2                (DEBUG_SELECT_ACTIVE | DEBUG_SELECT_CH2)
#define MODE_CH_OFF              (DEBUG_SELECT_ACTIVE | DEBUG_SELECT_CH_OFF)

#define MODE_SIGNED              (SIGNED_DATA)

#define PEAK_START_DISABLE       (0x00ffffff)
#define PEAK_STOP_DISABLE        (0x00ffffff)


#define DISABLE_INTERRUPT       (0)
#define ENABLE_INTERRUPT       	(1)

#define DMA_LENGTH				(128*1024*1024)

#define IND_DEBUG_READ        	(1)
#define IND_DEBUG_WRITE       	(2)
#define IND_DEBUG_DMA_READ    	(3)
#define IND_DEBUG_DMA_WRITE   	(4)

/*
** Status constants
*/
#define BIT_SPI_BUSY            (0x01)
#define BIT_S2MM_ERR            (0x02)
#define BIT_MM2S_RD_CMPLT       (0x04)
#define BIT_MM2S_ERR            (0x08)
#define BIT_SPI_ERR             (0x10)
#define BIT_INTERRUPT_ACTIVE    (0x20)
#define BIT_FPGA_RESET_STATUS   (0x40)
#define BIT_ADC_TEST_STATUS     (0x80)
#define BIT_PPS_DEBUG_STATUS    (0x100)
#define BIT_DMA_RESET_STATUS    (0x200)
#define BIT_DMA_DEBUG_STATUS    (0x400)
#define BIT_INTERRUPT_EN_STATUS (0x800)
#define BIT_BAT_LOW_STATUS      (0x1000)
#define BIT_AC_OK_STATUS        (0x2000)

#define STAT_NOT_OS_RUNNING     (0x00010000)        // unused -- could be used as feedback?
#define STAT_NOT_RESTART_REQ    (0x00020000)        // PM MCU has requested a restart
#define STAT_NOT_SHUTDOWN_REQ   (0x00040000)        // PM MCU has requested a shutdown
#define STAT_NOT_SPARE_MCU      (0x00080000)        // a spare signal to PM MCU (could be input or output)?

#define STAT_SPARE_1            (0x00100000)        // spare signal to IND3 RF board (assume output for now -- could be used for feedback)?
#define STAT_SPARE_2            (0x00200000)        // spare signal to IND3 RF board (assume output for now -- could be used for feedback)?
#define STAT_SPARE_3            (0x00300000)        // spare signal to IND3 RF board (assume output for now -- could be used for feedback)?
#define STAT_SPARE_4            (0x00400000)        // spare signal to IND3 RF board (assume output for now -- could be used for feedback)?

/*
 * CTRL bit definitions
 */
#define CTRL_RESET_3G         	(0x01)
#define CTRL_POWER_3G         	(0x02)
#define CTRL_EN_SELECT        	(0x04)

#define CTRL_NOT_OS_RUNNING     (0x00010000)        // output low to inidcate to PM MCU that we aer up and running ok
#define CTRL_NOT_RESTART_REQ    (0x00020000)
#define CTRL_NOT_SHUTDOWN_REQ   (0x00040000)
#define CTRL_NOT_SPARE_MCU      (0x00080000)        // a spare signal to PM MCU (could be input or output)?

#define CTRL_SPARE_1            (0x00100000)        // spare signal to IND3 RF board (assume output for now !!)
#define CTRL_SPARE_2            (0x00200000)        // spare signal to IND3 RF board (assume output for now !!)
#define CTRL_SPARE_3            (0x00400000)        // spare signal to IND3 RF board (assume output for now !!)
#define CTRL_SPARE_4            (0x00800000)        // spare signal to IND3 RF board (assume output for now !!)

/*
 * LED definitions
 */
#define LED_RUNNING           	(0x01)
#define LED_ALERT             	(0x02)
#define LED_SPARE             	(0x04)
#define LED_PPS_OK            	(0x08)
#define LED_3G_OK             	(0x10)
#define LED_WS_OK             	(0x20)

/*
 * SPI definitions
 */
#define SPI_CTRL_WRITE        	(0x0)
#define SPI_CTRL_READ         	(0x20000)
#define SPI_DEVICE_AD9467     	(0x0)
#define SPI_DEVICE_AD9517     	(0x10000)




enum IND_user_cmds
{
   IND_USER_RESET,
   IND_USER_DMA_RESET,
   IND_USER_SET_MODE,
   IND_USER_SET_ADDRESS,
   IND_USER_DMA_TEST,
   IND_USER_TRIG_PPS,
   IND_USER_SPI_ACCESS,
   IND_USER_STATUS,
   IND_USER_SET_LEDS,       /* use IND_USER_MODIFY_LEDS to set and clear */
   IND_USER_CLEAR_LEDS,     /* use IND_USER_MODIFY_LEDS to set and clear */
   IND_USER_SET_CTRL,       /* use IND_USER_MODIFY_CTRL to set and clear */
   IND_USER_CLEAR_CTRL,     /* use IND_USER_MODIFY_CTRL to set and clear */
   IND_USER_SET_INTERRUPT,
   IND_USER_GET_SEM,
   IND_USER_SET_SEM,
   IND_USER_REG_DEBUG,
   IND_USER_MODIFY_LEDS,
   IND_USER_MODIFY_CTRL,
   IND_USER_READ_MAXMIN,
   IND_USER_FPGA_VERSION
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
   __u32                            peak_detect_start;
   __u32                            peak_detect_end;
} ;

/*
 *  Structure to set and clear bits for the following ioctl commands.
 *      IND_USER_MODIFY_LEDS
 *      IND_USER_MODIFY_CTRL
 */
typedef struct IND_bit_flag_struct {
   __u32                            set;
   __u32                            clear;
   __u32                            toggle;
} IND_bit_flag_t;


struct IND_spi_cmd_struct {
   __u32                           port_device[16];
   __u32                           port_addr[16];
   __u32                           port_data[16];
   __u32                           num_spi_writes;
   __u32                           num_spi_reads;
} ;

struct IND_debug_struct {
   __u32                           cmd;
   __u32                           reg;
   __u32                           data;
} ;

struct IND_maxmin_struct {
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

/*
 *  Structure to get the FPGA version.
 *      reserved -- upper 16 bits.
 *          set to zeros.
 *      version  -- lower 16 bits.
 *          version_major -- bits [15:8]
 *          version_minor -- bits [7:0]
 */
struct IND_fpga_version_struct {
   __u16                           _reserved_0;
   __u8                            major;
   __u8                            minor;
} __packed;


#define IND_IOCTL_BASE	't'

#define IND_USER_RESET              _IOWR(IND_IOCTL_BASE, 0x80, struct IND_cmd_struct)
#define IND_USER_DMA_RESET          _IOWR(IND_IOCTL_BASE, 0x81, struct IND_cmd_struct)
#define IND_USER_SET_MODE           _IOWR(IND_IOCTL_BASE, 0x82, struct IND_cmd_struct)
#define IND_USER_SET_ADDRESS        _IOWR(IND_IOCTL_BASE, 0x83, struct IND_cmd_struct)
#define IND_USER_DMA_TEST           _IOWR(IND_IOCTL_BASE, 0x84, struct IND_cmd_struct)
#define IND_USER_TRIG_PPS           _IOWR(IND_IOCTL_BASE, 0x85, struct IND_cmd_struct)
#define IND_USER_SPI_ACCESS         _IOWR(IND_IOCTL_BASE, 0x86, struct IND_cmd_struct)
#define IND_USER_STATUS             _IOWR(IND_IOCTL_BASE, 0x87, struct IND_cmd_struct)
#define IND_USER_SET_LEDS           _IOWR(IND_IOCTL_BASE, 0x88, struct IND_cmd_struct)
#define IND_USER_CLEAR_LEDS         _IOWR(IND_IOCTL_BASE, 0x89, struct IND_cmd_struct)
#define IND_USER_SET_CTRL           _IOWR(IND_IOCTL_BASE, 0x8a, struct IND_cmd_struct)
#define IND_USER_CLEAR_CTRL         _IOWR(IND_IOCTL_BASE, 0x8b, struct IND_cmd_struct)
#define IND_USER_SET_INTERRUPT      _IOWR(IND_IOCTL_BASE, 0x8c, struct IND_cmd_struct)
#define IND_USER_GET_SEM            _IOWR(IND_IOCTL_BASE, 0x8d, struct IND_cmd_struct)
#define IND_USER_SET_SEM            _IOWR(IND_IOCTL_BASE, 0x8e, struct IND_cmd_struct)
#define IND_USER_REG_DEBUG          _IOWR(IND_IOCTL_BASE, 0x8f, struct IND_cmd_struct)
#define IND_USER_MODIFY_LEDS        _IOWR(IND_IOCTL_BASE, 0x90, struct IND_bit_flag_struct)
#define IND_USER_MODIFY_CTRL        _IOWR(IND_IOCTL_BASE, 0x91, struct IND_bit_flag_struct)
#define IND_USER_READ_MAXMIN        _IOWR(IND_IOCTL_BASE, 0x92, struct IND_maxmin_struct)
#define IND_USER_FPGA_VERSION       _IOWR(IND_IOCTL_BASE, 0x93, struct IND_fpga_version_struct)

#endif /* _IND_H */
