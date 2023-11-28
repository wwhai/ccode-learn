#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define TUN_DEV "/dev/net/tun"

int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open(TUN_DEV, O_RDWR)) < 0)
    {
        perror("Error opening /dev/net/tun");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

    if (*dev)
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        perror("Error creating TUN device");
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

int main()
{
    char tun_name[IFNAMSIZ];
    int tun_fd, n;
    char buffer[1500];

    // 分配一个虚拟网卡
    strcpy(tun_name, "mytun0");
    tun_fd = tun_alloc(tun_name);

    if (tun_fd < 0)
    {
        exit(1);
    }

    printf("Allocated interface %s\n", tun_name);

    // 在这里你可以设置虚拟网卡的 IP 地址等配置

    // 向虚拟网卡写入数据
    strcpy(buffer, "helloworld");
    n = write(tun_fd, buffer, strlen(buffer));

    if (n < 0)
    {
        perror("Error writing to tun");
        close(tun_fd);
        exit(1);
    }

    printf("Wrote %d bytes to tun\n", n);

    close(tun_fd);
    return 0;
}
