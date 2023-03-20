#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "hello"
#define BUFFER_SIZE 1024

static char message[BUFFER_SIZE];
static int message_len = 0;

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device released\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;
    if (*offset >= message_len)
    {
        return 0;
    }
    if (length > message_len - *offset)
    {
        length = message_len - *offset;
    }
    bytes_read = copy_to_user(buffer, message + *offset, length);
    *offset += length;
    return length - bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
    if (length > BUFFER_SIZE)
    {
        return -EINVAL;
    }
    if (copy_from_user(message, buffer, length))
    {
        return -EFAULT;
    }
    message_len = length;
    return length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init hello_init(void)
{
    int ret = 0;
    printk(KERN_INFO "Hello, world\n");
    ret = register_chrdev(0, DEVICE_NAME, &fops);
    if (ret < 0)
    {
        printk(KERN_ALERT "Failed to register character device\n");
        return ret;
    }
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye, world\n");
    unregister_chrdev(0, DEVICE_NAME);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver");