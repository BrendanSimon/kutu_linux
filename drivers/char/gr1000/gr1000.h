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

#define COTDR_DATA_SIZE 2
#define BOTDA_DATA_SIZE 4
#define ROTDR_DATA_SIZE 8

#define DMA_BUSY_FLAG      0x80
#define DDR_RESET_FLAG     0x40
#define UI_RESET_FLAG      0x20
#define DDR_CAL_FLAG       0x10
#define SPI_BUSY_FLAG      0x08
#define FIFO_EMPTY_FLAG    0x04
#define SWEEP_RUNNING_FLAG 0x02
#define TEST_RUNNING_FLAG  0x01

#define SPI_MAX_WAIT_COUNT 1000000
#define MAX_WAIT_COUNT     10000

/*
** System constants
*/
#define FPGA_RESET               0x01
#define START_TEST               0x02
#define STOP_TEST                0x04
#define SWEEP_START              0x08

#define MODE_BITS                0x07
#define BOTDA_MODE               0x00
#define BOTDA_MODE_ALT           0x01
#define COTDR_MODE_ALT           0x02
#define COTDR_MODE               0x03
#define COTDR_MODE_DUAL          0x04
#define ROTDR_MODE               0x05
#define TEST_DATA_ENABLE         0x08
#define DAC_UPDATE_ENABLE        0x10
#define CLK_200_MODE             0x20
#define DMA_ACTIVE               0x40
#define DMA_DEBUG                0x80
#define SWEEP_MODE               0x100
#define SWEEP_TEST_MODE          0x200
#define TEST_CLK_SELECT          0x400
#define SMA_CLK_ENABLE           0x800

#define REPEAT_LOOP              0x80000000
#define INTERRUPT_ACTIVE         0x40000000
#define REPEAT_COUNT             0x000003ff

#define RUN_BOTDA                (BOTDA_MODE|DAC_UPDATE_ENABLE|START_TEST)
#define RUN_BOTDA_TEST           (BOTDA_MODE|TEST_DATA_ENABLE|DAC_UPDATE_ENABLE|START_TEST)
#define RUN_COTDR                COTDR_MODE|START_TEST
#define RUN_COTDR_TEST           (COTDR_MODE|TEST_DATA_ENABLE|START_TEST)
#define RUN_ROTDR                ROTDR_MODE|START_TEST
#define RUN_ROTDR_TEST           (ROTDR_MODE|TEST_DATA_ENABLE|START_TEST)
#define RUN_NO_TEST              0

#define FREQUENCY_COUNTER_ACTIVE (TEST_CLK_SELECT)
#define FREQUENCY_TEST_ACTIVE    (SWEEP_TEST_MODE|TEST_CLK_SELECT)
#define EOM_ACTIVE               (SWEEP_TEST_MODE)

#define DEFAULT_ROW_STRIDE       0x00100000

#define DMA_LENGTH	(64*1024*1024)

#define GR1000_DEBUG_READ           1
#define GR1000_DEBUG_WRITE          2
#define GR1000_DEBUG_DMA_READ       3
#define GR1000_DEBUG_DMA_WRITE      4


enum gr1000_user_cmds
{
   GR1000_USER_RESET,
   GR1000_USER_SET_CLK,
   GR1000_USER_SET_PULSE,
   GR1000_USER_SET_ADC_OFFSET,
   GR1000_USER_SET_ROW_STRIDE,
   GR1000_USER_RUN_TEST,
   GR1000_USER_STATUS,
   GR1000_USER_READ_DATA,
   GR1000_USER_READ_FREQUENCY,
   GR1000_USER_SPI_WRITE,
   GR1000_USER_RUN_SWEEP,
   GR1000_USER_READ_DAC_TABLE,
   GR1000_USER_WRITE_DAC_TABLE
};

/*
 *  struct GR1000_registers.
 *  This structure points to the first block where the registers are located
 */

struct GR1000_cmd_struct {
   __u32                           config;
   __u32                           run_test;
   __u32                           botda_start_freq;
   __u32                           botda_end_freq;
   __u32                           adc_count;
   __u32                           botda_count;
   __u32                           row_stride;
} ;

struct GR1000_continuous_cmd_struct {
   __u32                           config;
   __u32                           run_test;
   __u32                           start_column;
   __u32                           end_column;
   __u32                           adc_count;
   __u32                           row_stride;
} ;


struct GR1000_pulse_struct {
   __u32                           pulse_width_AOM;
   __u32                           pulse_width_EOM;
   __u32                           pulse_width_gain;
   __u32                           pulse_rate;
   __u32                           pulse_delay;
} ;

struct GR1000_sweep_cmd_struct {
   __u32                           run_test;
   __u32                           initial_dac_value;
   __u32                           final_dac_value;
   __u32                           dac_step_size;
   __u32                           *sweep_data;
} ;

struct GR1000_spi_cmd_struct {
   __u32                           port_addr[16];
   __u32                           port_data[16];
   __u32                           num_spi_writes;
} ;

struct GR1000_read_data_struct {
   __u32                         *data;
   __u32                         column;
   __u32                         row;
   __u32                         num_columns;
   __u32                         num_rows;
   __u32                         row_stride;
   __u32                         datatype;
   __u32                         dma_addr_offset;
   __u32                         dma_active;
} ;

struct GR1000_debug_struct {
   __u32                           cmd;
   __u32                           reg;
   __u32                           data;
} ;


#define GR1000_IOCTL_BASE	't'

#define GR1000_USER_RESET              _IOWR(GR1000_IOCTL_BASE, 0x81, struct GR1000_cmd_struct)
#define GR1000_USER_SET_CLK            _IOWR(GR1000_IOCTL_BASE, 0x82, struct GR1000_cmd_struct)
#define GR1000_USER_SET_PULSE          _IOWR(GR1000_IOCTL_BASE, 0x83, struct GR1000_cmd_struct)
#define GR1000_USER_SET_ADC_OFFSET     _IOWR(GR1000_IOCTL_BASE, 0x84, struct GR1000_cmd_struct)
#define GR1000_USER_SET_ROW_STRIDE     _IOWR(GR1000_IOCTL_BASE, 0x85, struct GR1000_cmd_struct)
#define GR1000_USER_RUN_TEST           _IOWR(GR1000_IOCTL_BASE, 0x86, struct GR1000_cmd_struct)
#define GR1000_USER_STATUS             _IOWR(GR1000_IOCTL_BASE, 0x87, struct GR1000_cmd_struct)
#define GR1000_USER_READ_DATA          _IOWR(GR1000_IOCTL_BASE, 0x88, struct GR1000_cmd_struct)
#define GR1000_USER_READ_FREQUENCY     _IOWR(GR1000_IOCTL_BASE, 0x89, struct GR1000_cmd_struct)
#define GR1000_USER_SPI_WRITE          _IOWR(GR1000_IOCTL_BASE, 0x8a, struct GR1000_cmd_struct)
#define GR1000_USER_CONFIG_SWEEP       _IOWR(GR1000_IOCTL_BASE, 0x8b, struct GR1000_cmd_struct)
#define GR1000_USER_RUN_SWEEP          _IOWR(GR1000_IOCTL_BASE, 0x8c, struct GR1000_cmd_struct)
#define GR1000_USER_READ_DAC_TABLE     _IOWR(GR1000_IOCTL_BASE, 0x8d, struct GR1000_cmd_struct)
#define GR1000_USER_WRITE_DAC_TABLE    _IOWR(GR1000_IOCTL_BASE, 0x8e, struct GR1000_cmd_struct)
#define GR1000_INTERRUPT_COUNT         _IOWR(GR1000_IOCTL_BASE, 0x8f, struct GR1000_cmd_struct)
#define GR1000_DMA_BLOCK_COUNT         _IOWR(GR1000_IOCTL_BASE, 0x90, struct GR1000_cmd_struct)
#define GR1000_CONTINUOUS_SCAN         _IOWR(GR1000_IOCTL_BASE, 0x91, struct GR1000_cmd_struct)
#define GR1000_DMA_STATUS              _IOWR(GR1000_IOCTL_BASE, 0x92, struct GR1000_cmd_struct)
#define GR1000_DMA_TEST                _IOWR(GR1000_IOCTL_BASE, 0x93, struct GR1000_cmd_struct)
#define GR1000_REG_DEBUG               _IOWR(GR1000_IOCTL_BASE, 0x94, struct GR1000_cmd_struct)

#endif /* _GR1000_H */
