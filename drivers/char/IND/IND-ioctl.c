/* The industrial I/O core
 *
 * Copyright (c) 2015 Greg Smart
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Handling of device specific ioctls.
 *
 *
 * Things to look at here.
 * - create generic link into driver
 *
 */
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>

#include <linux/iio/iio.h>
#include "IND.h"
#include "IND_system.h"
#include <linux/iio/sysfs.h>
#include <linux/iio/buffer.h>

#define DEBUG

//
// IND_Set_User_Mode()
//
// Set the user operation mode
//
int IND_Set_User_Mode(struct IND_drvdata *IND, u32 arg)
{
   if (arg & (~(ADC_TEST_DATA|PPS_DEBUG_MODE|DMA_DEBUG_MODE))) {
      printk(KERN_DEBUG "IND_USER_SET_MODE: invalid argument\n");
      return -EFAULT;
   }

   IND->config_state |= arg;
   IND_write_reg(IND, R_MODE_CONFIG_ADDR, IND->config_state);

   return 0;
}

//
// IND_SPI_Write()
//
// Write a command to SPI port
// 3byte_mode only affects dac port (SPI 2)
//
int IND_SPI_Write(struct IND_drvdata *IND, void *user_ptr)
{
   u32   i,j,data;

   struct IND_spi_cmd_struct  cmd;

   if (copy_from_user(&cmd, user_ptr, sizeof(cmd))) {
      printk(KERN_DEBUG "IND_SPI_Write: copy failed\n");

      return -EFAULT;
   }

   if (cmd.num_spi_writes == 0)
     return 0;

   if (cmd.num_spi_writes > 16)
      return -EFAULT;

   for (j = 0; j < cmd.num_spi_writes; j++) {
      //
      // Wait for SPI access to finish
      //
      i = 0;
      while ((IND_Status(IND) & BIT_SPI_BUSY) && (i < MAX_WAIT_COUNT))
         i++;
#ifdef DEBUG
      printk(KERN_DEBUG "Looped through SPI wait %d times\n",i);
#endif
      if (cmd.port_addr[j] > 16)
         return -EFAULT;

      data =  (cmd.port_addr[j] << 28)|(cmd.port_data[j]&0x0fffffff);
      IND_write_reg(IND, R_SPI_DATA_ADDR, data);
      IND_write_reg(IND, R_SPI_DEVICE_ADDR, (cmd.port_device[j]&0x03));

      // wait until SPI write completes
      i = 0;
      while ((IND_Status(IND) & BIT_SPI_BUSY) && (i < MAX_WAIT_COUNT))
         i++;

   }
   return 0;
}

//
// IND_Run_Scan()
//
// Set the user operation mode
//
int IND_Run_Scan(struct IND_drvdata *IND, void *user_ptr)
{
   struct IND_cmd_struct   cmd;
   u32                        config;

   if (copy_from_user(&cmd, user_ptr, sizeof(cmd))) {
      printk(KERN_DEBUG "IND_Set_Run_Scan: copy failed\n");

      return -EFAULT;
   }

   // set mode (dma_debug and reset disabled)
   config = cmd.config & (ADC_TEST_DATA|PPS_DEBUG_MODE);

   IND->config_state = config;
   IND_write_reg(IND, R_MODE_CONFIG_ADDR, config);

   IND_write_reg(IND, R_DMA_READ_ADDR, cmd.address);


   return 0;
}
