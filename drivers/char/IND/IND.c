/*
 * Wrapper Driver used to control a two-channel Xilinx DMA Engine
 */
#include <linux/dmaengine.h>

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/clk.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <asm/uaccess.h>
#include <linux/dma-mapping.h>
#include <xen/page.h>

#include <linux/slab.h>
#include <linux/amba/xilinx_dma.h>
#include <linux/platform_device.h>

#include "IND.h"
#include "IND_system.h"

#define DRIVER_NAME "IND"
//#define DRIVER_NAME "IND_pdrv"
#define MODULE_NAME "IND"
#define IND_DEVICES 1

LIST_HEAD( IND_full_dev_list );

static struct IND_drvdata *get_elem_from_list_by_inode(struct inode *i)
{
   struct list_head *pos;
   struct IND_drvdata *IND = NULL;

   list_for_each( pos, &IND_full_dev_list ) {
      struct IND_drvdata *tmp;
      tmp = list_entry( pos, struct IND_drvdata, dev_list );
      if (tmp->devt == i->i_rdev)
      {
         IND = tmp;
         break;
      }
   }
   return IND;
}

static int IND_open(struct inode *i, struct file *filp)
{
   struct IND_drvdata *IND;

   IND = container_of(i->i_cdev, struct IND_drvdata, cdev);

   IND->irq_count = 0;

   printk(KERN_DEBUG "<%s> file: open()\n", MODULE_NAME);
   filp->private_data = IND;
   return 0;
}

static int IND_release(struct inode *i, struct file *f)
{
   struct IND_drvdata *IND;

   IND = container_of(i->i_cdev, struct IND_drvdata, cdev);

//   IND_write_reg(IND, R_RUN_TEST, STOP_TEST);
//   IND_write_reg(IND, R_BOTDA_END_FREQ, 0);

   printk(KERN_DEBUG "<%s> file: close()\n", MODULE_NAME);
   return 0;
}

static int IND_mmap(struct file *filp, struct vm_area_struct *vma)
{
   struct IND_drvdata *IND = filp->private_data;
   int result;
   unsigned long requested_size;

   requested_size = vma->vm_end - vma->vm_start;

   printk(KERN_DEBUG "<%s> file: mmap()\n", MODULE_NAME);
   printk(KERN_DEBUG
         "<%s> file: memory size reserved: %d, mmap size requested: %lu\n",
         MODULE_NAME, DMA_LENGTH, requested_size);

   if (requested_size > DMA_LENGTH) {
      printk(KERN_DEBUG "<%s> Error: %d reserved != %lu requested)\n",
            MODULE_NAME, DMA_LENGTH, requested_size);

      return -EAGAIN;
   }

   vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

   result = remap_pfn_range(vma, vma->vm_start,IND->dma_handle >> PAGE_SHIFT, requested_size, vma->vm_page_prot);


   if (result) {
      printk(KERN_DEBUG "<%s> Error: in calling remap_pfn_range: returned %d\n",
            MODULE_NAME, result);

      return -EAGAIN;
   }

   printk(KERN_DEBUG "<%s> : mmap complete \n",MODULE_NAME);
   return 0;
}

static ssize_t IND_read(struct file *f, char __user * buf, size_t
      len, loff_t * off)
{
   /* printk(KERN_INFO "<%s> file: read()\n", MODULE_NAME); */
   struct IND_drvdata *IND;
   int      count;
   char     *ptr;

   if (len > 65536)
   {
      return 0;
   }
   IND = get_elem_from_list_by_inode(f->f_inode);

   count = len;
   ptr   = buf;

   while (count > 1024) {
      memcpy(ptr, IND->base + R_IND_FIFO_BASE, 1024);
      count -= 1024;
      ptr+= 1024;
   }
   memcpy(ptr, IND->base + R_IND_FIFO_BASE, count);

   return len;
}

static long IND_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
   struct IND_drvdata *IND = filp->private_data;
   //   int __user *ip = (int __user *)arg;
   void  *arg_ptr = (void *)arg;
   long  ret = 0;
   unsigned int s2mm_status;
//   struct IND_read_data_struct read_cmd;
   struct IND_debug_struct debug_cmd;

   //printk(KERN_DEBUG "<%s> ioctl: entered IND_ioctl\n", MODULE_NAME);

   switch (cmd) {
      case IND_USER_RESET:
         if (arg == FPGA_RESET) {
            IND_write_reg(IND, R_MODE_CONFIG_ADDR, FPGA_RESET);
            IND->config_state = FPGA_RESET;
         } else {
            IND_write_reg(IND, R_MODE_CONFIG_ADDR, 0);
            IND->config_state = 0;
         }
         return 0;

      case IND_USER_SET_CLK:
         /*
          * clock control not implemented yet
          */
         return 0;

      case IND_USER_SET_MODE:
         ret = IND_Set_User_Mode(IND, arg);
         return ret;

      case IND_USER_RUN_SCAN:
         ret = IND_Run_Scan(IND, arg_ptr);
         return ret;

      case IND_USER_DMA_TEST:
         if (arg >= 0x800000)
            return -EFAULT;

         // start read from memory
         IND_write_reg(IND, R_DMA_READ_ADDR, IND->dma_handle);

         // start write to memory
         IND_write_reg(IND, R_DMA_WRITE_ADDR, (IND->dma_handle + (DMA_LENGTH/2)));
         IND_write_reg(IND, R_DMA_SIZE_ADDR, arg);

         // set dma into loopback mode
         IND_write_reg(IND, R_MODE_CONFIG_ADDR, (IND->config_state|MODE_TRIGGER_DMA));

         printk(KERN_DEBUG "<%s> : started dma \n",MODULE_NAME);

         s2mm_status = IND_Status(IND);

         while(!(s2mm_status & BIT_MM2S_RD_CMPLT_STATUS)) {
            s2mm_status = IND_Status(IND);
         }

         // set configuration back to original state
         IND_write_reg(IND, R_MODE_CONFIG_ADDR, IND->config_state);

         return 0;

      case IND_USER_TRIG_PPS:
         if ((arg != MODE_TRIGGER_PPS)&&(arg != GENERATE_PPS))
            return -1;

         IND_write_reg(IND, R_MODE_CONFIG_ADDR, (IND->config_state|arg));

         return ret;

      case IND_USER_SPI_WRITE:
         ret = IND_SPI_Write(IND, arg_ptr);
         return ret;

      case IND_USER_STATUS:
         ret = IND_Status(IND);
         if (copy_to_user(arg_ptr, &ret, sizeof(u32))) {
            return -EFAULT;
         }
         return 0;

      case IND_USER_REG_DEBUG:

         if (copy_from_user(&debug_cmd, arg_ptr, sizeof(debug_cmd))) {
            printk(KERN_DEBUG "IND_REG_DEBUG: copy failed\n");

            return -EFAULT;
         }

         if (debug_cmd.cmd == IND_DEBUG_WRITE){
            IND_write_reg(IND, debug_cmd.reg, debug_cmd.data);
            return 0;
         }

         if (debug_cmd.cmd == IND_DEBUG_READ)
            debug_cmd.data = IND_read_reg(IND, debug_cmd.reg);

         if (copy_to_user(arg_ptr, &debug_cmd, sizeof(debug_cmd))) {
            return -EFAULT;
         }

         return 0;

      default:
         break;
   }

   return ret;
}

/**
 * IND_isr() - The main interrupt handler.
 * @irq:	The interrupt number.
 * @data:	Pointer to the driver data structure.
 * returns: IRQ_HANDLED after the interrupt is handled.
 **/
static irqreturn_t IND_isr(int irq, void *data)
{
   struct IND_drvdata *IND = data;

   spin_lock(&IND->lock);

   // clear interrupt
   IND_write_reg(IND, R_INTERRUPT_ADDR,CLEAR_INTERRUPT);

   IND->irq_count++;

   IND_write_reg(IND, R_INTERRUPT_ADDR,DISABLE_INTERRUPT);

   spin_unlock(&IND->lock);

   return IRQ_HANDLED;
}

static const struct file_operations IND_fops = {
   .owner = THIS_MODULE,
   .read = IND_read,
   .mmap = IND_mmap,
   .unlocked_ioctl = IND_ioctl,
   .open = IND_open,
   .release = IND_release,
};

static const struct of_device_id IND_of_match_table[] = {
   { .compatible = "kutu,axi4-IND-controller-1.00-a", (void *)&IND_fops },
   { },
};
MODULE_DEVICE_TABLE(of, IND_of_match_table);

static int IND_probe(struct platform_device *pdev)
{
   const struct of_device_id *id;
   struct resource *mem;
   struct IND_drvdata *IND;
   dev_t devt;
   int ret;
   struct device *dev;

   if (!pdev->dev.of_node)
      return -ENODEV;

   id = of_match_node(IND_of_match_table, pdev->dev.of_node);
   if (!id)
      return -EINVAL;

   IND = devm_kzalloc(&pdev->dev, sizeof(*IND), GFP_KERNEL);
   if (!IND)
      return -ENOMEM;

   dev_info(&pdev->dev, "Kutu IND trying to call platform get resource\n");

   mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
   IND->base = devm_ioremap_resource(&pdev->dev, mem);

   if (IS_ERR(IND->base))
      return PTR_ERR(IND->base);

   IND->config_state = 0;

   dev_info(&pdev->dev, "Kutu IND finished call to platform get resource\n");

   dev_info(&pdev->dev, "Kutu IND trying to allocate irq\n");

   IND->irq = platform_get_irq(pdev, 0);
   ret = devm_request_irq(&pdev->dev, IND->irq, &IND_isr, 0, dev_name(&pdev->dev), IND);
   if (ret) {
      dev_err(&pdev->dev, "No IRQ available");
      return ret;
   }

   dev_info(&pdev->dev, "Kutu IND successfully setup irq\n");

   platform_set_drvdata(pdev, IND);
   spin_lock_init(&IND->lock);
   mutex_init(&IND->mutex);

   IND->is_open = 0;
   IND->dma_done = 0;
   IND->error_status = 0;
   dev_info(&pdev->dev, "ioremap %pa to %p\n", &mem->start, IND->base);

   IND->clk = devm_clk_get(&pdev->dev, NULL);
   if (IS_ERR(IND->clk)) {
      ret = PTR_ERR(IND->clk);
      goto failed5;
   }
   dev_info(&pdev->dev, "Successfully got device clock\n");
   dev_info(&pdev->dev, "device clock rate is %ld\n", clk_get_rate(IND->clk));

   //
   // reset IND device
   //

   ret = alloc_chrdev_region(&devt, 0, IND_DEVICES, DRIVER_NAME);
   if (ret < 0)
      goto failed5;
   dev_info(&pdev->dev, "Successfully allocated chrdev region\n");

   IND->devt = devt;

   cdev_init(&IND->cdev, &IND_fops);
   IND->cdev.owner = THIS_MODULE;
   ret = cdev_add(&IND->cdev, devt, 1);
   if (ret) {
      dev_err(&pdev->dev, "cdev_add() failed\n");
      goto failed6;
   }

   IND->class = class_create(THIS_MODULE, DRIVER_NAME);
   if (IS_ERR(IND->class)) {
      dev_err(&pdev->dev, "failed to create class\n");
      goto failed6;
   }

   dev = device_create(IND->class, &pdev->dev, devt, IND, DRIVER_NAME);
   if (IS_ERR(dev)) {
      dev_err(&pdev->dev, "unable to create device\n");
      goto failed7;
   }
   dev_info(&pdev->dev, "Successfully created device\n");

   //
   // allocate mmap area
   //
   IND->dma_addr = dma_alloc_coherent(NULL, DMA_LENGTH, &IND->dma_handle, GFP_KERNEL);

   dev_info(&pdev->dev, "dma_addr = 0x%x, dma_handle = 0x%x\n",(u32)IND->dma_addr,(u32)IND->dma_handle);
   dev_info(&pdev->dev, "IND base = 0x%x\n",(u32)IND->base);

   if (!IND->dma_addr) {
      printk(KERN_ERR "<%s> Error: allocating dma memory failed\n", MODULE_NAME);

      ret = -ENOMEM;
      goto failed8;
   }
   dev_info(&pdev->dev, "Successfully allocated dma memory\n");

   //platform_driver_register(pdev);
   dev_info(&pdev->dev, "Kutu IND finished loading driver\n");

   return 0;

failed8:
   device_destroy(IND->class, IND->devt);
failed7:
   class_destroy(IND->class);
failed6:
   /* Unregister char driver */
   unregister_chrdev_region(devt, IND_DEVICES);
failed5:

   return ret;
}

static int IND_remove(struct platform_device *pdev)
{
   struct IND_drvdata *IND;

   IND = platform_get_drvdata(pdev);

   if (!IND)
      return -ENODEV;

   unregister_chrdev_region(IND->devt, IND_DEVICES);

   //	sysfs_remove_group(&pdev->dev.kobj, &IND_attr_group);

   device_destroy(IND->class, IND->devt);
   class_destroy(IND->class);
   cdev_del(&IND->cdev);
   clk_unprepare(IND->clk);

   /* free mmap area */
   if (IND->dma_addr) {
      dma_free_coherent(NULL, DMA_LENGTH, IND->dma_addr, IND->dma_handle);
   }

   return 0;
}

static struct platform_driver IND_driver = {
   .probe = IND_probe,
   .remove = IND_remove,
   .driver = {
      .name = "IND",
      .of_match_table = IND_of_match_table,
   },
};
module_platform_driver(IND_driver);

MODULE_AUTHOR("Greg Smart <Greg.Smart@kutu.com.au>");
MODULE_DESCRIPTION("IND Linux driver");
MODULE_LICENSE("GPL v2");
