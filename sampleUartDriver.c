
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/module.h>


#define DRIVER_VERSION "v1.0"
#define DRIVER_AUTHOR "Greg Kroah-Hartman <greg@xxxxxxxxx>"
#define DRIVER_DESC "Tiny serial driver"

/* Module information */
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("GPL");

#define DELAY_TIME	HZ * 2	/* 2 seconds per character */
#define TINY_DATA_CHARACTER	't'

#define TINY_SERIAL_MAJOR	240	/* experimental range */
#define TINY_SERIAL_MINORS	1	/* only have one minor */
#define UART_NR	1	/* only use one port */

#define TINY_SERIAL_NAME	"ttytiny"

#define MY_NAME	TINY_SERIAL_NAME

#define dbg(fmt, arg...)	\
do {	\
if (debug)	\
printk (KERN_DEBUG "%s: %s: " fmt "\n",	\
MY_NAME , __FUNCTION__ , ## arg);	\
} while (0)
#define err(format, arg...) printk(KERN_ERR "%s: " format "\n" , MY_NAME , ## arg)
#define info(format, arg...) printk(KERN_INFO "%s: " format "\n" , MY_NAME , ## arg)
#define warn(format, arg...) printk(KERN_WARNING "%s: " format "\n" , MY_NAME , ## arg)

#define FUNC_ENTER() do { printk(KERN_INFO "Enter: %s\n", __func__); } while (0)


static int debug;

#if 0
static struct timer_list *timer;
#endif

static void tiny_stop_tx(struct uart_port *port)
{
FUNC_ENTER();
dbg ();
}

static void tiny_stop_rx(struct uart_port *port)
{
FUNC_ENTER();
dbg ();
}

static void tiny_enable_ms(struct uart_port *port)
{
FUNC_ENTER();
dbg ();
}

#if 0
static void tiny_tx_chars(struct uart_port *port)
{
struct circ_buf *xmit = &port->info->xmit;
int count;

dbg ();
if (port->x_char) {
dbg ("wrote %2x", port->x_char);
port->icount.tx++;
port->x_char = 0;
return;
}
if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
tiny_stop_tx(port, 0);
return;
}

count = port->fifosize >> 1;
do {
dbg ("wrote %2x", xmit->buf[xmit->tail]);
xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
port->icount.tx++;
if (uart_circ_empty(xmit))
break;
} while (--count > 0);

if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
uart_event(port, EVT_WRITE_WAKEUP);

if (uart_circ_empty(xmit))
tiny_stop_tx(port, 0);
}
#endif

static void tiny_start_tx(struct uart_port *port)
{
FUNC_ENTER();
dbg ();
}

#if 0
static void tiny_timer (unsigned long data)
{
struct uart_port *port;
struct tty_struct *tty;

dbg ();

port = (struct uart_port *)data;
if (!port)
return;
if (!port->info)
return;
tty = port->info->tty;
if (!tty)
return;

/* add one character to the tty port */
/* this doesn't actually push the data through unless tty->low_latency is set */
tty_insert_flip_char(tty, TINY_DATA_CHARACTER, 0);

tty_flip_buffer_push(tty);

/* resubmit the timer again */
timer->expires = jiffies + DELAY_TIME;
add_timer (timer);

/* see if we have any data to transmit */
tiny_tx_chars(port);
}
#endif

static unsigned int tiny_tx_empty(struct uart_port *port)
{
FUNC_ENTER();
return 0;
}

static unsigned int tiny_get_mctrl(struct uart_port *port)
{
FUNC_ENTER();
return 0;
}

static void tiny_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
FUNC_ENTER();
}

static void tiny_break_ctl(struct uart_port *port, int break_state)
{
FUNC_ENTER();
}

#if 0
static void
tiny_change_speed(struct uart_port *port, unsigned int cflag,
unsigned int iflag, unsigned int quot)
{
/* get the byte size */
switch (cflag & CSIZE) {
case CS5:
dbg ("data bits = 5");
break;
case CS6:
dbg ("data bits = 6");
break;
case CS7:
dbg ("data bits = 7");
break;
default: // CS8
dbg ("data bits = 8");
break;
}

/* determine the parity */
if (cflag & PARENB)
if (cflag & PARODD)
dbg (" - parity = odd\n");
else
dbg (" - parity = even\n");
else
dbg (" - parity = none\n");

/* figure out the stop bits requested */
if (cflag & CSTOPB)
dbg (" - stop bits = 2\n");
else
dbg (" - stop bits = 1\n");

/* figure out the flow control settings */
if (cflag & CRTSCTS)
dbg (" - RTS/CTS is enabled\n");
else
dbg (" - RTS/CTS is disabled\n");

/* Set baud rate */
//UART_PUT_DIV_LO(port, (quot & 0xff));
//UART_PUT_DIV_HI(port, ((quot & 0xf00) >> 8));
}
#endif

static int tiny_startup(struct uart_port *port)
{
FUNC_ENTER();
#if 0
/* this is the first time this port is opened */
/* do any hardware initialization needed here */

/* create our timer and submit it */
if (!timer) {
timer = kmalloc (sizeof (*timer), GFP_KERNEL);
if (!timer)
return -ENOMEM;
}
timer->data = (unsigned long )port;
timer->expires = jiffies + DELAY_TIME;
timer->function = tiny_timer;
add_timer (timer);
#endif
return 0;
}

static void tiny_shutdown(struct uart_port *port)
{
FUNC_ENTER();
#if 0
/* The port is being closed by the last user. */
/* Do any hardware specific stuff here */

/* shut down our timer */
del_timer (timer);
#endif
}

static const char *tiny_type(struct uart_port *port)
{
FUNC_ENTER();
return "tinytty";
}

static void tiny_release_port(struct uart_port *port)
{
FUNC_ENTER();
}

static int tiny_request_port(struct uart_port *port)
{
FUNC_ENTER();
return 0;
}

static void tiny_config_port(struct uart_port *port, int flags)
{
FUNC_ENTER();
}

static int tiny_verify_port(struct uart_port *port, struct serial_struct *ser)
{
FUNC_ENTER();
return 0;
}

static struct uart_ops tiny_ops = {
.tx_empty	= tiny_tx_empty,
.set_mctrl	= tiny_set_mctrl,
.get_mctrl	= tiny_get_mctrl,
.stop_tx	= tiny_stop_tx,
.start_tx	= tiny_start_tx,
.stop_rx	= tiny_stop_rx,
.enable_ms	= tiny_enable_ms,
.break_ctl	= tiny_break_ctl,
.startup	= tiny_startup,
.shutdown	= tiny_shutdown,
#if 0
.change_speed	= tiny_change_speed,
#endif
.type	= tiny_type,
.release_port	= tiny_release_port,
.request_port	= tiny_request_port,
.config_port	= tiny_config_port,
.verify_port	= tiny_verify_port,
};

static struct uart_port tiny_port = {
.ops	= &tiny_ops,
};

static struct uart_driver tiny_reg = {
.owner	= THIS_MODULE,
.driver_name	= TINY_SERIAL_NAME,
.dev_name	= TINY_SERIAL_NAME,
.major	= TINY_SERIAL_MAJOR,
.minor	= TINY_SERIAL_MINORS,
.nr	= UART_NR,
};


static int __init tiny_init(void)
{
int result;

info ("Tiny serial driver");

result = uart_register_driver(&tiny_reg);
if (result)
return result;

result = uart_add_one_port(&tiny_reg, &tiny_port);
if (result)
uart_unregister_driver(&tiny_reg);

return result;
}

static void __exit tiny_exit(void)
{
uart_remove_one_port(&tiny_reg, &tiny_port);
uart_unregister_driver(&tiny_reg);
}

module_init (tiny_init);
module_exit (tiny_exit);


My own vuart driver:
======================================================================
#define DEBUG 1

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/serial.h>
#include <linux/serial_core.h>

#define FUNC_ENTER() do { pr_info("ENTER: %s\n", __func__); } while (0)

struct vuart_dev {
struct uart_port port;
};

static const char driver_name[] = "vuart";
static const char tty_dev_name[] = "vtty";
static struct vuart_dev priv0, priv1;



static unsigned int vuart_tx_empty(struct uart_port *port)
{
FUNC_ENTER();
return 1;
}

static void vuart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
FUNC_ENTER();
}

static unsigned int vuart_get_mctrl(struct uart_port *port)
{
FUNC_ENTER();
return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
}

static void vuart_stop_tx(struct uart_port *port)
{
FUNC_ENTER();
}

static void vuart_start_tx(struct uart_port *port)
{
FUNC_ENTER();
}

static void vuart_stop_rx(struct uart_port *port)
{
FUNC_ENTER();
}

static void vuart_enable_ms(struct uart_port *port)
{
FUNC_ENTER();
}

static void vuart_break_ctl(struct uart_port *port, int break_state)
{
FUNC_ENTER();
}

static int vuart_startup(struct uart_port *port)
{
FUNC_ENTER();
return 0;
}

static void vuart_shutdown(struct uart_port *port)
{
FUNC_ENTER();
}

static void vuart_set_termios(struct uart_port *port,
struct ktermios *termios,
struct ktermios *old)
{
FUNC_ENTER();
}

static const char *vuart_type(struct uart_port *port)
{
FUNC_ENTER();
return "VUART";
}

static int vuart_request_port(struct uart_port *port)
{
FUNC_ENTER();
return 0;
}

static void vuart_config_port(struct uart_port *port, int flags)
{
FUNC_ENTER();
}

static void vuart_release_port(struct uart_port *port)
{
FUNC_ENTER();
}

static int vuart_verify_port(struct uart_port *port, struct serial_struct *ser)
{
FUNC_ENTER();
return 0;
}

static struct uart_ops vuart_uart_ops = {
.tx_empty	= vuart_tx_empty,
.set_mctrl	= vuart_set_mctrl,
.get_mctrl	= vuart_get_mctrl,
.stop_tx	= vuart_stop_tx,
.start_tx	= vuart_start_tx,
.stop_rx	= vuart_stop_rx,
.enable_ms	= vuart_enable_ms,
.break_ctl	= vuart_break_ctl,
.startup	= vuart_startup,
.shutdown	= vuart_shutdown,
.set_termios	= vuart_set_termios,
.type	= vuart_type,
.release_port	= vuart_release_port,
.request_port	= vuart_request_port,
.config_port	= vuart_config_port,
.verify_port	= vuart_verify_port,
};

static struct uart_driver vuart_driver = {
.owner	= THIS_MODULE,
.driver_name	= driver_name,
.dev_name	= tty_dev_name,
.major	= 240,
.minor	= 0,
.nr	= 2,
};

static int __init
vuart_init (void)
{
int ret;

ret = uart_register_driver(&vuart_driver);

if (ret) {
pr_err("%s: could not register UART driver\n", driver_name);
goto out_register_driver;
}

memset(&priv0, sizeof(struct vuart_dev), 0);
priv0.port.line = 0;
priv0.port.ops = &vuart_uart_ops;
spin_lock_init(&priv0.port.lock);
ret = uart_add_one_port(&vuart_driver, &priv0.port);

if (ret) {
pr_err("%s: could not add port vuart0\n", driver_name);
goto out_add_port0;
}

memset(&priv1, sizeof(struct vuart_dev), 0);
priv1.port.line = 1;
priv1.port.ops = &vuart_uart_ops;
spin_lock_init(&priv1.port.lock);
ret = uart_add_one_port(&vuart_driver, &priv1.port);

if (ret) {
pr_err("%s: could not add port vuart1\n", driver_name);
goto out_add_port1;
}

printk ("Module %s loaded\n", driver_name);
return 0;

out_add_port1:
uart_remove_one_port(&vuart_driver, &priv0.port);
out_add_port0:
uart_unregister_driver(&vuart_driver);
out_register_driver:
return ret;
}

static void __exit
vuart_exit (void)
{
uart_remove_one_port(&vuart_driver, &priv0.port);
uart_remove_one_port(&vuart_driver, &priv1.port);
uart_unregister_driver(&vuart_driver);
printk ("Module %s removed\n", driver_name);
}

MODULE_AUTHOR("Ira W. Snyder <iws@xxxxxxxxxxxxxxxx>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");

module_init(vuart_init);
module_exit(vuart_exit);