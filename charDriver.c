/* character.c – Our First Driver code */

// Reference - http://www.opensourceforu.com/page/2/?s=drivers
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>	
#include <linux/uaccess.h>
 
 // tail -f /var/log/syslog - use this to view the messages

/*

A simple cat can be done as read operation - request to read
echo can be considered as a request to write operation

*/


 //A static variable inside a function keeps its value between invocations.

static dev_t first; // Global variable for the first device number 
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
static char c;	
/*
Static - 

Is used widely as an "access control" feature. If you have a .c file implementing 
some functionality, it usually exposes only a few "public" functions to users. The 
rest of its functions should be made static, so that the user won't be able to access 
them. This is encapsulation, a good practice.
*/
static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}
  static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: read()\n");
    if (copy_to_user(buf, &c, len) != 0)
        return -EFAULT;
    else
        return 1;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: write()\n");
    if (copy_from_user(&c, buf + len - 1, 1) != 0)
        return -EFAULT;
    else
        return len;
}
 

  static struct file_operations pugs_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .read = my_read,
  .write = my_write
};
 
 // **  An application gets connected to a device file by invoking the open system call on the device file.


/*
The connection between the application and the device file is based on the name of 
the device file. However, the connection between the device file and the device driver 
is based on the number of the device file, not the name
*/


/*

Connecting the device file with the device driver involves two steps:

1) Registering for the <major, minor> range of device files.
2) Linking the device file operations to the device driver functions.

*/
static int __init t1_init(void) /* Constructor */
{
	if (alloc_chrdev_region(&first, 0, 3, "abel") < 0) // Get the major and minor number dynamically - " " is the device name, here abel 
    {
        return -1;
    }

    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) // Create a character driver class 
	{
	    unregister_chrdev_region(first, 1); // return a range of device numbers
	    // This function will unregister a range of count device numbers, starting with from
	    return -1;
	}


	if (device_create(cl, NULL, first, NULL, "mynull") == NULL) // create device file  , here the name is " mynull ""
    {
	    class_destroy(cl);
	    unregister_chrdev_region(first, 1);
	    return -1;
    }

    cdev_init(&c_dev, &pugs_fops); // initialize a cdev structure
    // Initializes cdev, remembering fops, making it ready to add to the system with cdev_add.

    if (cdev_add(&c_dev, first, 1) == -1) //cdev_add — add a char device to the system
    {
	    device_destroy(cl, first);
	    class_destroy(cl);
	    unregister_chrdev_region(first, 1);
	    return -1;
    }
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first)); 
    // check cat /proc/devices for the registered device also ls /dev for registered device file
    printk(KERN_INFO "Hello World\n");
    return 0;
}
 
static void __exit t1_exit(void) /* Destructor */
{
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
    printk(KERN_INFO "I am Dead\n");
}
 
module_init(t1_init);
module_exit(t1_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abel Paul Babu");
MODULE_DESCRIPTION("t1");


/*

The connection between the application and the device file is based on the name of the device file. However, 
the connection between the device file and the device driver is based on the number of the device file, not 
the name. This allows a user-space application to have any name for the device file, and enables the kernel-space 
to have a trivial index-based linkage between the device file and the device driver. This device file number is 
more commonly referred to as the <major, minor> pair, or the major and minor numbers of the device file.

*/





/*



1. Application
2. Character device file
3. Character device driver
4. Character device


 major and minor number connect the application and the character device file
 file to driver connection is done by linking the functions.

*/