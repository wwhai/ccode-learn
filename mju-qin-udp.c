void f()
{
    // 创建UDP套接字
    udp->fd = socket(AF_INET, SOCK_DGRAM, 0);
    // 创建UDP套接字，并设置为非阻塞模式
    // udp->fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (udp->fd < 0)
    {
        perror("socket");
        exit(1);
    }

    // 设置超时选项
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(udp->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 设置源地址
    memset(&udp->source_addr, 0, sizeof(udp->source_addr));
    udp->source_addr.sin_family = AF_INET;
    udp->source_addr.sin_addr.s_addr = inet_addr(SOURCE_BORAD_IP);
    udp->source_addr.sin_port = htons(SOURCE_BORAD_PORT);

    // 设置目标地址信息
    memset(&udp->dest_addr, 0, sizeof(udp->dest_addr));
    udp->dest_addr.sin_family = AF_INET;
    udp->dest_addr.sin_port = htons(udp->dest_port);
    if (inet_pton(AF_INET, udp->dest_ip, &(udp->dest_addr.sin_addr)) <= 0)
    {
        perror("inet_pton");
        exit(1);
    }

    // 设置源地址信息
    memset(&udp->source_addr, 0, sizeof(udp->source_addr));
    udp->source_addr.sin_family = AF_INET;
    udp->source_addr.sin_port = htons(SOURCE_BORAD_PORT);
    // udp->source_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // udp->source_addr.sin_addr.s_addr = inet_addr(SOURCE_BORAD_IP);

    // // 设置地址复用模式
    // int reuse = 1;
    // if (setsockopt(udp->fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    //   perror("setsockopt");
    //   exit(1);
    // }

    // 设置端口复用模式
    int reuse_port = 1;
    if (setsockopt(udp->fd, SOL_SOCKET, 0, &reuse_port, sizeof(reuse_port)) < 0)
    { // SO_REUSEPORT
        perror("setsockopt(SO_REUSEPORT) failed");
        exit(EXIT_FAILURE);
    }

    // 绑定源地址
    if (bind(udp->fd, (struct sockaddr *)&udp->source_addr, sizeof(udp->source_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
}