#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define NETLINK_USER 31

int main()
{
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    int sock_fd, ret;

    // 创建 netlink socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
    if (sock_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置源地址
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    // 绑定 socket
    if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 设置目的地址
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;    // 发送给内核
    dest_addr.nl_groups = 0; // 不使用多播

    // 创建 netlink 消息
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(256));
    memset(nlh, 0, NLMSG_SPACE(256));
    nlh->nlmsg_len = NLMSG_SPACE(256);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    // 设置消息内容
    strcpy(NLMSG_DATA(nlh), "add@/sys/devices/pci0000:00/0000:00:14.0/usb1/1-11/1-11:1.0/ttyUSB0");
    // strcpy(NLMSG_DATA(nlh), "hello");
    // 设置发送数据
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    //
    struct msghdr msg = {0};
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    // 发送消息
    ret = sendmsg(sock_fd, &msg, 0);
    if (ret < 0)
    {
        perror("sendmsg");
        exit(EXIT_FAILURE);
    }

    printf("Sent message to kernel: %s\n", (char *)NLMSG_DATA(nlh));

    // 释放资源
    free(nlh);
    close(sock_fd);

    return 0;
}
