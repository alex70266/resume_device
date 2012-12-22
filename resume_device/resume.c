#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fcntl.h>  // for O_NONBLOCK
#include <linux/err.h>
#include <asm/uaccess.h>   // for put_user
#include <linux/sched.h>   // for wait queue, TASK_INTERRUPTIBLE
#include <linux/cdev.h>
#include <linux/init.h>
#include "chardev.h"


#define DEVICE_NAME "resume"  //Dev name as it appears in "/proc/devices"
#define BUF_LEN 80

static dev_t devno=0;
static struct cdev mycdev;
static char Message[BUF_LEN];
DECLARE_WAIT_QUEUE_HEAD(char_wq);
static int bEmpty=0;
static int Major=0;


/* ********************** device operation ************************* */
static int device_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *off)
{
		

	char *from;
        from = Message;

	if ( *off >= BUF_LEN ){
		printk("<1> Good to see you,This is a device driver used for presentation\n");
		printk("<1> I am Yu-Yang Lin. I am looking for job about Embedded SW/FW\n");
		printk("<1> I had trained in III about Linux base Embedded System software devlope.\n");
		printk("<1> Here is My resume link:\n");
		printk("<1> http://db.tt/YzZNQxXJ   (PDF) \n");
			
		return 0;
	}
	if ( *off + length >= BUF_LEN )
      		length = BUF_LEN;

	if (filp->f_flags & O_NONBLOCK) {
		if(strlen(from)==0)
		      return -EAGAIN;
  	}
	else 
	{
		if( strlen(from)==0) {
      			bEmpty=0;
      			wait_event_interruptible(char_wq, bEmpty);
    		}

    		if( signal_pending(current) ) {
			return -EINTR;
		}
	}
  
	
	if( copy_to_user( buffer, from, length ) )
    		return -EFAULT;
	*off += length;
	
	
	return length;
}

static ssize_t device_write(struct file *filp,const char *buffer, size_t length, loff_t *off)
{

	printk("<1> hi\n");
	printk("<1> You can use cat %s to read for My introduction\n",DEVICE_FILE_NAME);	
	printk("<1> ****************************************\n");
	printk("<1> ****************************************\n");
	printk("<1> ****************************************\n");

	char *to;
       	to=Message;
	if ( *off >= BUF_LEN )
		return 0;
	if ( *off + length >= BUF_LEN )
		length = BUF_LEN;


	if( copy_from_user( to, buffer, length ) )
		return -EFAULT;


	if ( !(filp->f_flags & O_NONBLOCK) ) {
		bEmpty=1;
		wake_up_interruptible(&char_wq);
	}  
	
	*off += length;

	return length;
}


/*    **********************   DEViCE I/O CONCTRL declare ***************** */
int device_ioctl(struct inode *inode, struct file *file,unsigned int ioctl_num, unsigned long ioctl_param)
{
	

	switch(ioctl_num){

	case IOCTL_CLEAR_MSG:
		memset(Message, '\0', BUF_LEN);
		printk("<1> Message cleaned\n");
		break;
	}
	
	return 0;
}

static struct file_operations fops={
	.read = device_read,
	.write = device_write,

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.ioctl = device_ioctl,
#else
	.unlocked_ioctl = device_ioctl,	
#endif
	.open = device_open,
	.release = device_release,

};


/* ************* init module and exit module ***********************/

static int __init start_module(void)
{

	if(Major) {
		if ( register_chrdev_region(MKDEV(Major,0), 1, DEVICE_NAME) < 0 ) {
			printk ("register_chrdev_region() fail\n");
			return -1;
		}
	}

	else {
		if (alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME) < 0) {
			printk ("alloc_chrdev_region() fail\n");
			return -1;
		}
		Major=MAJOR(devno);
	}

	cdev_init(&mycdev, &fops);
	mycdev.owner=THIS_MODULE;
	
	if(cdev_add(&mycdev, MKDEV(Major,0), 1)) {
		printk ("Error adding cdev\n");
	}

	memset(Message, '\0', BUF_LEN);


	printk("<1> I was assigned major number %d for %s and created this device\n", Major,DEVICE_FILE_NAME);
	printk("<1> You can use echo \"(your name)\" > %s to sent you name to Char Driver \n",DEVICE_FILE_NAME);
	printk("<1> ****************************************\n");
	printk("<1> ****************************************\n");
	printk("<1> ****************************************\n");

	return 0;
}

static void __exit stop_module(void) 
{ 
	printk("<1> Thanks for your reading \nI really hope to have am opportunity to co-work with your company\n");
	printk("<1> ****************************************\n");
	printk("<1> ****************************************\n");
	printk("<1> ****************************************\n");
	cdev_del(&mycdev);
	unregister_chrdev_region(MKDEV(Major, 0), 1);
}


module_init(start_module);
module_exit(stop_module);

MODULE_LICENSE("GPL");

