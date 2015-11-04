/*
 *  gr1000_system.h -- Register definitions for GR1000 implementation
 *
 *  Greg Smart
 *
 *  Version 0.1 10/03/14
 *
 */

/*--------------------------------------------------------------------
 *
 *  GR1000 register include file
 *  This file contains all the macros and defines needed by the driver
 *  and user programs using it.
 *
 *--------------------------------------------------------------------*/

#ifndef _GR1000_SYSTEM_H
#define _GR1000_SYSTEM_H

/* general register memory location */

#define GR1000_BASE              0x43C00000

#define R_GR1000_REG_BASE        0x0000
#define R_GR1000_FIFO_BASE       0x0800
#define R_GR1000_BRAM_BASE       0x1000

#define R_GR1000_STATUS          0x0000

#define R_DMA_WRITE_ADDR         0x0000
#define R_DMA_READ_ADDR          0x0004
#define R_MODE_CONFIG_ADDR       0x0008
#define R_INTERRUPT_ADDR         0x000C

#define R_SPI_DATA_ADDR          0x0010   // read address on 64 byte boundaries
#define R_SPI_DEVICE_ADDR        0x0014

#define GR1000_REG_BASE          (GR1000_BASE + R_GR1000_REG_BASE)
#define GR1000_FIFO_BASE         (GR1000_BASE + R_GR1000_FIFO_BASE)
#define GR1000_BRAM_BASE         (GR1000_BASE + R_GR1000_BRAM_BASE)

#define GR1000_STATUS            (GR1000_BASE + R_GR1000_STATUS)

#define DMA_WRITE_ADDR           (GR1000_BASE + R_DMA_WRITE_ADDR)
#define DMA_READ_ADDR            (GR1000_BASE + R_DMA_READ_ADDR)
#define MODE_CONFIG_ADDR         (GR1000_BASE + R_MODE_CONFIG_ADDR)
#define INTERRUPT_ADDR           (GR1000_BASE + R_INTERRUPT_ADDR)
#define SPI_DATA_ADDR            (GR1000_BASE + R_SPI_DATA_ADDR)
#define SPI_DEVICE_ADDR          (GR1000_BASE + R_SPI_DEVICE_ADDR)

/*
** configuration constants
*/
#define GENERATE_PPS             0x01
#define DEBUG_START_DMA          0x02
#define PPS_DEBUG_MODE           0x40
#define DMA_DEBUG_MODE           0x80

#define MODE_NORMAL              0x00
#define MODE_DMA_DEBUG           (DMA_DEBUG_MODE)
#define MODE_TRIGGER_DMA         (DMA_DEBUG_MODE|DEBUG_START_DMA)
#define MODE_PPS_DEBUG           (PPS_DEBUG_MODE)
#define MODE_TRIGGER_PPS         (PPS_DEBUG_MODE|GENERATE_PPS)

/*
 * interrupt constants
 */
#define ENABLE_INTERRUPT         0x01
#define DISABLE_INTERRUPT        0x02
#define CLEAR_INTERRUPT          0x03

/*
** SPI constants
*/
#define SPI_PORT_LOW             0x01
#define SPI_PORT_HIGH            0x02
#define SPI_PORT_BOTH            0x03

/*
** Status constants
*/
#define BIT_SPI_BUSY             0x01
#define BIT_S2MM_ERR             0x02
#define BIT_MM2S_RD_CMPLT        0x04
#define BIT_MM2S_ERR             0x08
#define BIT_SPI_ERR              0x10
#define BIT_INTERRUPT_ACTIVE     0x20
#define BIT_S2MM_ERR_STATUS      0x40
#define BIT_MM2S_RD_CMPLT_STATUS 0x80
#define BIT_MM2S_ERR_STATUS      0x100

#define SPI_MAX_WAIT_COUNT 1000000
#define MAX_WAIT_COUNT     10000


/*
 *  struct GR1000_axi_lite_base.
 *  This structure provides general access for the 3 blocks within the system
 */

struct GR1000_axi_lite_base {
   u32                           registers[512];
   u32                           fifo[512];
   u32                           bram[1024];
} ;

/*
 *  struct GR1000_registers.
 *  This structure points to the first block where the registers are located
 */

struct GR1000_registers {
   u32                           pulse_rate_gen;      // 0x0
   u32                           pulse_width_gen;     // 0x4
   u32                           sweep_test;          // 0x8
   u32                           run_test;            // 0xC
   u32                           data_read_addr;      // 0x10
   u32                           data_read_stride;    // 0x14
} ;

#define MAX_DEVICES     4

/**
 * struct gr1000_drvdata - Device Configuration driver structure
 *
 * @dev: Pointer to the device structure
 * @cdev: Instance of the cdev structure
 * @devt: Pointer to the dev_t structure
 * @class: Pointer to device class
 * @fclk_class: Pointer to fclk device class
 * @dma_done: The dma_done status bit for the DMA command completion
 * @error_status: The error status captured during the DMA transfer
 * @irq: Interrupt number
 * @clk: Peripheral clock for devcfg
 * @fclk: Array holding references to the FPGA clocks
 * @fclk_exported: Flag inidcating whether an FPGA clock is exported
 * @is_open: The status bit to indicate whether the device is opened
 * @sem: Instance for the mutex
 * @lock: Instance of spinlock
 * @base_address: The virtual device base address of the device registers
 * @is_partial_bitstream: Status bit to indicate partial/full bitstream
 */
struct gr1000_drvdata {
   struct device *dev;
   struct cdev cdev;
   dev_t devt;
   struct class *class;
   int irq;
   uint32_t irq_count;
   int dma_block_count;
   struct clk *clk;
   volatile bool dma_done;
   volatile int error_status;
   bool is_open;
   struct mutex mutex;
   spinlock_t lock;
   void __iomem *base;
   void __iomem *dma_base;
   uint32_t config_state;
   char *dma_addr;
   dma_addr_t dma_handle;
   struct GR1000_read_data_struct repeat_read_cmd;

	struct list_head dev_list;
};

static inline void gr1000_write_reg(struct gr1000_drvdata *gr1000, unsigned int reg, uint32_t val)
{
	writel(val, gr1000->base + reg);
}

static inline uint32_t gr1000_read_reg(struct gr1000_drvdata *gr1000, unsigned int reg)
{
	return(readl(gr1000->base + reg));
}

static inline void gr1000dma_write_reg(struct gr1000_drvdata *gr1000, unsigned int reg, uint32_t val)
{
	writel(val, gr1000->dma_base + reg);
}

static inline uint32_t gr1000dma_read_reg(struct gr1000_drvdata *gr1000, unsigned int reg)
{
	return(readl(gr1000->dma_base + reg));
}

//
//
//
//
//  Prototypes
//
//
//
//
//
//
//
//
//
//
//

u32 GR1000_Status(struct gr1000_drvdata *gr1000);

//
// GR1000_Open()
//
// Open GR1000 system and mmap registers for user access.
//
// Returns pointer to virtual address.
//
u32 GR1000_Open(u32 init_fpga);

//
// GR1000_Close()
//
// Close GR1000 system and unmap memory.  Function is called with
// virtual address that was returned when opened.
//
u32 GR1000_Close(int fd);

//
// GR1000_Set_Pulse()
//
// Setup the pulse generator
//
int GR1000_Set_Pulse(struct gr1000_drvdata *gr1000, void *user_ptr);

//
// GR1000_Set_Adc_Offset()
//
// Set the ADC DC offset
//
u32 GR1000_Set_Adc_Offset(int arg1);

//
// GR1000_Set_Row_Stride()
//
// Set the memory row stride
//
u32 GR1000_Set_Row_Stride(int arg1);

//
// GR1000_Run Test()
//
// Initiate a COTDR, BOTBA or ROTDR test
//
// Function will return immediately after initiating test
// before test completion
//
int GR1000_Run_Test(struct gr1000_drvdata *gr1000, void *user_ptr);

//
// GR1000_Continuous_Scan()
//
// Initiate a continuous COTDR test
//
// Function will return immediately after initiating test
//
// Continuous scan uses 1024 scans looping around continually
// For default setup of 1Mbyte row_stride this means it will work
// with 1Gbyte of DDR3 memory
//
// Can support either 1 or 2 adc channels, at this point support 1 only
// until further testing.
//
// This command sets up a looping test.  Every 128 scans an interrupt is
// generated. The ISR calls GR1000_Read_data which initiates a DMA transfer
// of the selected column block to system memory. Only one data block is
// currently supported.
//
// After the command is initiated, the write_block counter is incremented
// There are 8 memory blocks (8Mbytes each).  For a single channel transfer
// the maximum nuber of columns is (32768 - 32).  The column number must be
// on a boundary of 32 in increments of 32.
//
// Once started, the user only has to monitor the write_block counter to check
// when a new block is available.
//
int GR1000_Continuous_Scan(struct gr1000_drvdata *gr1000, void *user_ptr);

//
// GR1000_Status()
//
// read the GR1000 status registers
//
// status if system is running test, sweep or is idle
//
//static inline u32 GR1000_Status(struct gr1000_drvdata *gr1000);

//
// GR1000_Read_Data()
//
// Read a 2-dimensional block of data from test
// data is available immediately
// -1 is returned if error, number of bytes is returned if ok.
//
int GR1000_Read_Data(struct gr1000_drvdata *gr1000, struct GR1000_read_data_struct *cmd);

//
// GR1000_Read_Frequency()
//
// Read frequency of index (-1 if error)
//
int GR1000_Read_Frequency(struct gr1000_drvdata *gr1000, void *arg_ptr);

//
// GR1000_SPI_Write()
//
// Write a command to SPI port
//
int GR1000_SPI_Write(struct gr1000_drvdata *gr1000, void *user_ptr);

//
// GR1000_Run_Sweep()
//
// Run a frequency sweep
// cmd determines number of dac values to sweep.
// This is normally 800,65536,or 262144
//
int GR1000_Run_Sweep(struct gr1000_drvdata *gr1000, void *arg_ptr);

//
// GR1000_Read_Dac_Table()
//
// This function reads the DAC table back from the
// FPGA.  It is useful for comparing table values to
// calculated values
//
int GR1000_Read_DAC_Table(struct gr1000_drvdata *gr1000, u32 *read_data);

//
// GR1000_Write_Dac_Table()
//
// This function writes the DAC table to the FPGA.
// Table values are 20 bit words.  Values > 2^20 are
// written as 0xFFFFF,
//
int GR1000_Write_DAC_Table(struct gr1000_drvdata *gr1000, u32 *write_data);

#endif /* _GR1000_SYSTEM_H */
