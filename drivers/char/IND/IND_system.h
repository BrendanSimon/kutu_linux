/*
 *  IND_system.h -- Register definitions for IND implementation
 *
 *  Greg Smart
 *
 *  Version 0.1 11/11/15
 *
 */

/*--------------------------------------------------------------------
 *
 *  IND register include file
 *  This file contains all the macros and defines needed by the driver
 *  and user programs using it.
 *
 *--------------------------------------------------------------------*/

#ifndef _IND_SYSTEM_H
#define _IND_SYSTEM_H

/* general register memory location */

#define IND_BASE              0x43C00000

#define R_IND_REG_BASE        0x0000
#define R_IND_FIFO_BASE       0x0800
#define R_IND_BRAM_BASE       0x1000

#define R_IND_STATUS          0x0000

#define R_DMA_WRITE_ADDR         0x0000
#define R_DMA_READ_ADDR          0x0004
#define R_DMA_SIZE_ADDR          0x0008
#define R_MODE_CONFIG_ADDR       0x000C
#define R_INTERRUPT_ADDR         0x0010

#define R_SPI_DATA_ADDR          0x0014   // read address on 64 byte boundaries
#define R_SPI_DEVICE_ADDR        0x0018
#define R_CAPTURE_COUNT_ADDR     0x001C

#define IND_REG_BASE          (IND_BASE + R_IND_REG_BASE)
#define IND_FIFO_BASE         (IND_BASE + R_IND_FIFO_BASE)
#define IND_BRAM_BASE         (IND_BASE + R_IND_BRAM_BASE)

#define IND_STATUS            (IND_BASE + R_IND_STATUS)

#define DMA_WRITE_ADDR           (IND_BASE + R_DMA_WRITE_ADDR)
#define DMA_READ_ADDR            (IND_BASE + R_DMA_READ_ADDR)
#define MODE_CONFIG_ADDR         (IND_BASE + R_MODE_CONFIG_ADDR)
#define INTERRUPT_ADDR           (IND_BASE + R_INTERRUPT_ADDR)
#define SPI_DATA_ADDR            (IND_BASE + R_SPI_DATA_ADDR)
#define SPI_DEVICE_ADDR          (IND_BASE + R_SPI_DEVICE_ADDR)
#define SPI_CAPTURE_COUNT_ADDR   (IND_BASE + R_SPI_DEVICE_ADDR)

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
 *  struct IND_axi_lite_base.
 *  This structure provides general access for the 3 blocks within the system
 */

struct IND_axi_lite_base {
   u32                           registers[512];
   u32                           fifo[512];
   u32                           bram[1024];
} ;

/*
 *  struct IND_registers.
 *  This structure points to the first block where the registers are located
 */

struct IND_registers {
   u32                           pulse_rate_gen;      // 0x0
   u32                           pulse_width_gen;     // 0x4
   u32                           sweep_test;          // 0x8
   u32                           run_test;            // 0xC
   u32                           data_read_addr;      // 0x10
   u32                           data_read_stride;    // 0x14
} ;

#define MAX_DEVICES     4

/**
 * struct IND_drvdata - Device Configuration driver structure
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
struct IND_drvdata {
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
   uint32_t config_state;
   char *dma_addr;
   dma_addr_t dma_handle;
	struct list_head dev_list;
};

static inline void IND_write_reg(struct IND_drvdata *IND, unsigned int reg, uint32_t val)
{
	writel(val, IND->base + reg);
}

static inline uint32_t IND_read_reg(struct IND_drvdata *IND, unsigned int reg)
{
	return(readl(IND->base + reg));
}

//
// IND_Status()
//
// read the FOS status registers
//
// status if system is running test, sweep or is idle
//
static inline u32 IND_Status(struct IND_drvdata *IND)
{
   u32 status;

   status = IND_read_reg(IND, R_IND_STATUS);
//   status &= 0x000001ff;

   return status;
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
// IND_Open()
//
// Open IND system and mmap registers for user access.
//
// Returns pointer to virtual address.
//
u32 IND_Open(u32 init_fpga);

//
// IND_Close()
//
// Close IND system and unmap memory.  Function is called with
// virtual address that was returned when opened.
//
u32 IND_Close(int fd);

//
// IND_Set_User_Mode()
//
// Setup the system configuration
//
int IND_Set_User_Mode(struct IND_drvdata *IND, u32 arg);


//
// IND_Run_Scan()
//
// Initiate a scan
//
// Function will return immediately after initiating test
// before test completion
//
int IND_Run_Scan(struct IND_drvdata *IND, void *user_ptr);

//
// IND_SPI_Write()
//
// Write a command to SPI port
//
int IND_SPI_Write(struct IND_drvdata *IND, void *user_ptr);


#endif /* _IND_SYSTEM_H */
