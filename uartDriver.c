
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/console.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h> 
#include <linux/uaccess.h>
#include <linux/serial_core.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <asm/io.h>
#include <linux/platform_device.h>


// ssh root@10.42.0.81
//g-serial vendor=0x8086 product=0xBABE

/*

Things to do 

 - Read 
    - docs/uartDriverImplementation.pdf
    - docs/useOfFunctions.pdf



*/



#define UART_MAJOR        200  /* You've to get this assigned */
#define UART_MINOR_START  70   /* Start minor numbering here */
#define my_uart_portS        1    /* The phone has 2 USB_UARTs */
#define USB_UART         30   /* UART type. Add this to

// read this mail trail - http://lkml.iu.edu/hypermail/linux/kernel/0808.0/2820.html
/* 
  Each USB_UART has a 3-byte register set consisting of
   UU_STATUS_REGISTER at offset 0, UU_READ_DATA_REGISTER at
   offset 1, and UU_WRITE_DATA_REGISTER at offset 2 as shown
   in Table 6.1 
*/
#define UART_BASE  0x9000F000/* Memory base for UART0 */
// 0x9000b000 0x9000F000
#define UART_REGISTER_SPACE  0x0b // a total of 11 registers - refer intel galileo datasheet

/* Semantics of bits in the status register */
#define UART_TX_FULL         0x02  /* TX FIFO is full */
#define UART_RX_EMPTY        0x08  /* TX FIFO is empty */
#define UART_STATUS          0x0F  /* Parity/frame/overruns? */

#define UART_IRQ            17     /* USB_UART1 IRQ refer - http://www.webopedia.com/quick_ref/IRQnumbers.asp*/
#define UART_FIFO_SIZE       16    /* FIFO size */
#define UART_CLK_FREQ        44236800

void uart_putc(struct uart_port *port, unsigned char c)
{
  while(__raw_readb(port->membase) & UART_TX_FULL);
  
  __raw_writeb(c, (port->membase+1));
}

static unsigned char uart_getc(struct uart_port *port)
{
  /* Wait until data is available in the RX_FIFO */
  while (__raw_readb(port->membase) & UART_RX_EMPTY);

  /* Obtain the data */
  return(__raw_readb(port->membase+2));
}

static unsigned char uart_status(struct uart_port *port)
{
  return(__raw_readb(port->membase) & UART_STATUS);
}

unsigned int more_chars_to_be_read(void)
{ return 0; } 

static irqreturn_t uart_rxint(int irq, void *dev_id)
//  interrupt handler or interrupt service routine (ISR)
// read this - http://www.makelinux.net/books/lkd2/?u=ch06lev1sec3
// static irqreturn_t intr_handler(int irq, void *dev_id, struct pt_regs *regs) 
{
  struct uart_port *port = (struct uart_port *) dev_id;
  struct tty_struct *tty = port->state->port.tty;
  // What is the use of this ? 

  unsigned int status, data;
  /* ... */
  do {
    /* ... */
    /* Read data */
    data   = uart_getc(port);
    /* Normal, overrun, parity, frame error? */
    status = uart_status(port);
    /* Dispatch to the tty layer */
    tty_insert_flip_char(tty, data, status);
    /* ... */
  } while (more_chars_to_be_read()); /* More chars */
  /* ... */
  tty_flip_buffer_push(tty);

  return IRQ_HANDLED;
}


static void uart_start_tx(struct uart_port *port)
{
  printk(KERN_INFO "uart_start_tx\n");
  while(1)
  {
    uart_putc(port, port->state->xmit.buf[port->state->xmit.tail]);
    port->state->xmit.tail = (port->state->xmit.tail+1) & (UART_XMIT_SIZE-1);
    port->icount.tx++;
    if(uart_circ_empty(&port->state->xmit)) 
      break;
  }
}

static int uart_startup(struct uart_port *port)
{
  int retval = 0;
  printk(KERN_INFO "uart_startup\n");
  /* Request IRQ */
  if ((retval = request_irq(port->irq, uart_rxint, 0, "uart", (void *)port))) {
    // read http://www.makelinux.net/books/lkd2/ch06lev1sec3
    /*int request_irq(unsigned int irq,
                      irqreturn_t (*handler)(int, void *, struct pt_regs *),
                      unsigned long irqflags,
                      const char *devname,
                      void *dev_id)
    */
    return retval;
  }
  /* ... */
  return retval;
}

static void uart_shutdown(struct uart_port *port)
{
  printk(KERN_INFO "uart_shutdown\n");
  free_irq(port->irq, port);
}

static const char * uart_type(struct uart_port *port)
{
  char *C;
  printk(KERN_INFO "uart_type\n");
  return NULL;
}

// uart_type called when accessing - /proc/tty/driver/uart file

static void uart_config_port(struct uart_port *port, int flags)
{
  printk(KERN_INFO "uart_config_port, flags: %d \n",flags);
  if (flags & UART_CONFIG_TYPE)
    port->type = PORT_16550A;
  printk(KERN_INFO "Port type set\n");  
}

static int uart_request_port(struct uart_port *port)
{
  printk(KERN_INFO "uart_request_port\n");
  struct resource *retval;
  retval = request_mem_region(port->mapbase, (u32)UART_REGISTER_SPACE, "uart");
  if(retval)
    return -EBUSY;
  return 0;
}

static void uart_release_port(struct uart_port *port)
{
  printk(KERN_INFO "uart_release_port\n");
  release_mem_region(port->mapbase, UART_REGISTER_SPACE);
}



static struct uart_driver uart_reg = {
  .owner          = THIS_MODULE,          /* Owner */
  .driver_name    = "uart",               /* Driver name */
  .dev_name       = "ttyUU",              /* Node name */
  .major          = UART_MAJOR,       /* Major number */
  .minor          = UART_MINOR_START, /* Minor number start */
  .nr             = my_uart_portS,       /* Number of UART ports */
  //.cons           = &usb_uart_console,    
                                          /* Pointer to the console
                                             structure. Discussed in Chapter
                                             12, "Video Drivers" */
};


static struct uart_ops my_uart_ops = {
  .start_tx     = uart_start_tx,    /* Start transmitting */
  .startup      = uart_startup,     /* App opens USB_UART */
  .shutdown     = uart_shutdown,    /* App closes USB_UART */
  .type         = uart_type,        /* Set UART type */
  .config_port  = uart_config_port, /* Configure when driver
                                           adds a USB_UART port */
  .request_port = uart_request_port,/* Claim resources
                                           associated with a
                                           USB_UART port */
  .release_port = uart_release_port,/* Release resources
                                           associated with a
                                           USB_UART port */
#if 0    
  .tx_empty     = usb_uart_tx_empty,    /* Transmitter busy? */
  .set_mctrl    = usb_uart_set_mctrl,   /* Set modem control */
  .get_mctrl    = usb_uart_get_mctrl,   /* Get modem control
  .stop_tx      = usb_uart_stop_tx,     /* Stop transmission */
  .stop_rx      = usb_uart_stop_rx,     /* Stop reception */
  .enable_ms    = usb_uart_enable_ms,   /* Enable modem status
                                           signals */
  .set_termios  = usb_uart_set_termios, /* Set termios */
#endif
};

static struct uart_port my_uart_port = {
  
    .mapbase  = (unsigned int) UART_BASE,
    .iotype   = UPIO_MEM,           /* Memory mapped */
    .irq      = UART_IRQ,      /* IRQ  */
    .uartclk  = UART_CLK_FREQ,  /* Clock HZ */
    .fifosize = UART_FIFO_SIZE, /* Size of the FIFO */
     .ops      = &my_uart_ops,      /* UART operations */
    .flags    = UPIO_MEM, //UPF_BOOT_AUTOCONF,  /* UART port flag */
    // this was changed to UPIO_mem check the net for more info
    .line     = 0,                  /* UART port number */
  // flag is referenced from this site - http://lxr.free-electrons.com/source/include/linux/serial_core.h#L148
};



/************* platform driver *************/
int uart_plat_suspend(struct platform_device *dev, pm_message_t s)
{
  uart_suspend_port(&uart_reg, &my_uart_port);
  return 0;
}
int uart_plat_resume(struct platform_device *dev)
{
  uart_resume_port(&uart_reg, &my_uart_port);
  return 0;
}
int uart_plat_probe(struct platform_device *dev)
{
  /* Use 'usb_uart_driver' to drive 'usb_uart_port[i]' */
  uart_add_one_port(&uart_reg, &my_uart_port);
  platform_set_drvdata(dev, &my_uart_port);
  return 0;
}
int uart_plat_remove(struct platform_device *dev)
{
  platform_set_drvdata(dev, NULL);
  /* remove port to uart driver */
  uart_remove_one_port(&uart_reg, &my_uart_port);
  return 0;
}
struct platform_driver uart_plat_driver =
{
  .probe  = uart_plat_probe,
  .remove = uart_plat_remove,
  .resume = uart_plat_resume,
  .suspend = uart_plat_suspend,
  .driver = { 
    .owner = THIS_MODULE,
    .name = "uart" },
};

struct platform_device *uart_plat_device;

/********* init and exit *********/
int __init uart_init(void)
{
  int retval;
  /* register uart driver */
  printk(KERN_INFO "The process has started \n");
    if ((retval = uart_register_driver(&uart_reg))) {
       printk(KERN_INFO "Error Registering Driver, Error Code :%d\n",retval);
      return retval;
    }

  /* register platform device, 
     usually called during arch-specific setup */
  uart_plat_device = platform_device_register_simple("uart", 0, NULL, 0);

  if(IS_ERR(uart_plat_device)) 
  {
    uart_unregister_driver(&uart_reg);
    printk(KERN_INFO "Error platform simple registeration \n");
    return PTR_ERR(uart_plat_device);
  }

  
  /* Announce a matching plat-driver for plat-dev registered above */
  retval = platform_driver_register(&uart_plat_driver);
  if(retval) {
    uart_unregister_driver(&uart_reg);
    platform_device_unregister(uart_plat_device);
    printk(KERN_INFO "Error platform registeration \n");
    return -EFAULT;
  }
  printk(KERN_INFO " I am alive !! \n");
  
  return 0;
}
module_init(uart_init);

void __exit uart_exit(void)
{
  printk(KERN_INFO " I am dead !! \n");
}
module_exit(uart_exit);

// /*
// static int __init my_uart_init(void)
// {
//    int retval;
//    /* Register the USB_UART driver with the serial core */
//    printk(KERN_INFO "The process has started \n");
//    if ((retval = uart_register_driver(&uart_reg))) {
//       printk(KERN_INFO "Error Registering Driver, Error Code :%d\n",retval);
//       return retval;
//    }
//     printk(KERN_INFO "successfully Added Driver, Error Code :%d\n",retval);
//    if((retval = uart_add_one_port(&uart_reg, &my_uart_port)))
//    {
//       printk(KERN_INFO "Error Adding Port, Error Code :%d\n",retval);
//       uart_unregister_driver(&uart_reg);
//       return retval;

//    }
//   printk(KERN_INFO "successfully Added Port, Error Code :%d\n",retval);
//    printk(KERN_INFO " I am alive !! \n");
//    return 0;
// }

// /* Driver Exit */
// static void __exit my_uart_exit(void)
// {
//  /* Unregister the USB_UART driver */
//   uart_remove_one_port(&uart_reg, &my_uart_port);
//   uart_unregister_driver(&uart_reg);
//   printk(KERN_INFO " I am dead !! \n");
// }

// module_init(my_uart_init);
// module_exit(my_uart_exit);



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abel Paul Babu");
MODULE_DESCRIPTION("t1");