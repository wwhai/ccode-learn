#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/tty_driver.h>
#include <linux/tty_port.h>

static struct tty_driver *serial_driver;

static void serial_receive(struct tty_port *port, const unsigned char *buf, char *c, int count)
{
    // 处理从串口接收到的数据，这里简单地将数据打印到内核日志
    printk(KERN_INFO "Received from serial port: %.*s\n", count, buf);

    // 这里可以进行更复杂的处理，比如将数据发送到网络层或其他子系统
}

static int serial_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
    struct tty_port *port = tty->port;

    // 处理向串口写入的数据，这里简单地将数据打印到内核日志
    printk(KERN_INFO "Write to serial port: %.*s\n", count, buf);

    // 这里可以进行更复杂的处理，比如将数据发送到网络层或其他子系统

    // 通知 tty 层数据已发送完成
    tty_flip_buffer_push(tty);

    return count; // 返回写入的字节数
}

static struct tty_operations serial_ops = {
    .receive_buf = serial_receive,
    .write = serial_write,
};

static int __init serial_init(void)
{
    // 分配 tty 驱动
    serial_driver = alloc_tty_driver(1);
    if (!serial_driver)
    {
        printk(KERN_ALERT "Failed to allocate tty driver\n");
        return -ENOMEM;
    }

    // 设置 tty 驱动属性
    serial_driver->owner = THIS_MODULE;
    serial_driver->driver_name = "serial_driver";
    serial_driver->name = "ttyS0";
    serial_driver->major = TTY_MAJOR;
    serial_driver->type = TTY_DRIVER_TYPE_SERIAL;
    serial_driver->subtype = SERIAL_TYPE_NORMAL;
    serial_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;

    // 设置 tty 操作
    serial_driver->ops = &serial_ops;

    // 注册 tty 驱动
    if (tty_register_driver(serial_driver))
    {
        printk(KERN_ALERT "Failed to register tty driver\n");
        put_tty_driver(serial_driver);
        return -ENOMEM;
    }

    printk(KERN_INFO "Serial driver initialized\n");

    return 0;
}

static void __exit serial_exit(void)
{
    // 注销 tty 驱动
    tty_unregister_driver(serial_driver);

    printk(KERN_INFO "Serial driver exited\n");
}

module_init(serial_init);
module_exit(serial_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Serial Driver in Kernel Mode");
