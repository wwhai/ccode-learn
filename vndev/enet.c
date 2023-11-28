#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/in.h>
#include <linux/ip.h>

#define DRIVER_NAME "my_net_driver"

static struct net_device *netdev;

static int my_net_open(struct net_device *dev)
{
    printk(KERN_INFO "my_net_driver: device opened\n");
    netif_start_queue(dev);
    return 0;
}

static int my_net_stop(struct net_device *dev)
{
    printk(KERN_INFO "my_net_driver: device closed\n");
    netif_stop_queue(dev);
    return 0;
}

static netdev_tx_t my_net_xmit(struct sk_buff *skb, struct net_device *dev)
{
    printk(KERN_INFO "my_net_driver: transmitting data\n");

    // 在这里处理发送的数据

    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

static struct net_device_stats *my_net_stats(struct net_device *dev)
{
    // 在这里处理统计信息
    return NULL;
}

static int my_net_rx(struct sk_buff *skb, struct net_device *dev, struct packet_type *pt, struct net_device *orig_dev)
{
    printk(KERN_INFO "my_net_driver: received data\n");

    // 在这里处理接收到的数据

    dev_kfree_skb(skb);
    return 0;
}

static const struct net_device_ops my_net_ops = {
    .ndo_open = my_net_open,
    .ndo_stop = my_net_stop,
    .ndo_start_xmit = my_net_xmit,
    .ndo_get_stats = my_net_stats,
};
static void my_net_setup(struct net_device *dev)
{
    // 设置指定的 MAC 地址
    unsigned char mac_addr[ETH_ALEN] = {0x00, 0x15, 0x5d, 0xc4, 0xb6, 0x73};
    memcpy(dev->dev_addr, mac_addr, ETH_ALEN);
    memcpy(dev->broadcast, "\xff\xff\xff\xff\xff\xff", ETH_ALEN);

    // 分配 IP 地址
    dev->ip_ptr = netdev_priv(dev);
    dev->ip_ptr->ifa_list = kmalloc(sizeof(struct in_device), GFP_KERNEL);
    if (dev->ip_ptr->ifa_list)
    {
        dev->ip_ptr->ifa_list->ifa_address = kmalloc(sizeof(struct in_ifaddr), GFP_KERNEL);
        if (dev->ip_ptr->ifa_list->ifa_address)
        {
            dev->ip_ptr->ifa_list->ifa_address->ifa_local = in_aton("192.168.1.2");
            dev->ip_ptr->ifa_list->ifa_address->ifa_mask = in_aton("255.255.255.0");
        }
    }
}

static int __init my_net_init(void)
{
    netdev = alloc_netdev(sizeof(struct in_device), DRIVER_NAME, NET_NAME_ENUM, my_net_setup);
    if (!netdev)
    {
        printk(KERN_ERR "my_net_driver: failed to allocate net device\n");
        return -ENOMEM;
    }

    netdev->netdev_ops = &my_net_ops;

    if (register_netdev(netdev))
    {
        printk(KERN_ERR "my_net_driver: failed to register net device\n");
        free_netdev(netdev);
        return -ENODEV;
    }

    dev_add_pack(&my_packet_type);

    printk(KERN_INFO "my_net_driver: module loaded\n");
    return 0;
}

static void __exit my_net_exit(void)
{
    dev_remove_pack(&my_packet_type);
    unregister_netdev(netdev);
    free_netdev(netdev);
    printk(KERN_INFO "my_net_driver: module unloaded\n");
}

module_init(my_net_init);
module_exit(my_net_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple network driver");
