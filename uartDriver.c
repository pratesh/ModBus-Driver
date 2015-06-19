
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h> 
#include <linux/uaccess.h>
#include <linux/serial_core.h>
#include <linux/tty.h>

// ssh root@10.42.0.81
//g-serial vendor=0x8086 product=0xBABE

/*

Things to do 

  - find type
  - find irq
  - check if base address is correct
  - hence fix the first line in error log
  - read this - http://www.tldp.org/LDP/nag2/x-087-2-serial-configuration.html
  - is the base address used by ttys0 if yes how to remove it
  - how to log in with ethernet cable 


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
#define UART_BASE   0x9000F000/* Memory base for UART0 */
// 0x9000b000 0x9000F000
#define UART_REGISTER_SPACE  0xb // a total of 11 registers - refer intel galileo datasheet

/* Semantics of bits in the status register */
#define UART_TX_FULL         0x02  /* TX FIFO is full */
#define UART_RX_EMPTY        0x08  /* TX FIFO is empty */
#define UART_STATUS          0x0F  /* Parity/frame/overruns? */

#define UART_IRQ            17     /* USB_UART1 IRQ refer - http://www.webopedia.com/quick_ref/IRQnumbers.asp*/
#define UART_FIFO_SIZE       16    /* FIFO size */
#define UART_CLK_FREQ        44236800





static void uart_start_tx(struct uart_port *port)
{
  printk(KERN_INFO "uart_start_tx\n");
}

static int uart_startup(struct uart_port *port)
{
  int retval = 0;
  printk(KERN_INFO "uart_startup\n");
  /* Request IRQ */
  if ((retval = request_irq(port->irq, usb_uart_rxint, 0,
                            "usb_uart", (void *)port))) {
    return retval;
  }
  /* ... */
  return retval;
}

static void uart_shutdown(struct uart_port *port)
{
  printk(KERN_INFO "uart_shutdown\n");
}

static const char * uart_type(struct uart_port *port)
{
  char *C;
  printk(KERN_INFO "uart_type\n");
  return C;
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
  return 0;
}

static void uart_release_port(struct uart_port *port)
{
  printk(KERN_INFO "uart_release_port\n");
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
    .line     = 0,                  /* UART port number */
  // flag is referenced from this site - http://lxr.free-electrons.com/source/include/linux/serial_core.h#L148
};





static int __init my_uart_init(void)
{
   int retval;
   /* Register the USB_UART driver with the serial core */
   printk(KERN_INFO "The process has started \n");
   if ((retval = uart_register_driver(&uart_reg))) {
      printk(KERN_INFO "Error Registering Driver, Error Code :%d\n",retval);
      return retval;
   }
    printk(KERN_INFO "successfully Added Driver, Error Code :%d\n",retval);
   if((retval = uart_add_one_port(&uart_reg, &my_uart_port)))
   {
      printk(KERN_INFO "Error Adding Port, Error Code :%d\n",retval);
      uart_unregister_driver(&uart_reg);
      return retval;

   }
  printk(KERN_INFO "successfully Added Port, Error Code :%d\n",retval);
   printk(KERN_INFO " I am alive !! \n");
   return 0;
}

/* Driver Exit */
static void __exit my_uart_exit(void)
{
 /* Unregister the USB_UART driver */
  uart_remove_one_port(&uart_reg, &my_uart_port);
  uart_unregister_driver(&uart_reg);
  printk(KERN_INFO " I am dead !! \n");
}

module_init(my_uart_init);
module_exit(my_uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abel Paul Babu");
MODULE_DESCRIPTION("t1");