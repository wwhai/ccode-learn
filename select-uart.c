#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

int main() {
    int fd;
    struct termios config;

    // 打开串口设备
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Error opening serial port");
        return 1;
    }

    // 配置串口
    memset(&config, 0, sizeof(config));
    config.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    config.c_iflag = IGNPAR;
    config.c_oflag = 0;
    config.c_lflag = 0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &config);

    // 使用 select 函数检查串口是否可读
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 1;  // 设置超时时间为 1 秒
    timeout.tv_usec = 0;

    int ready = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if (ready == -1) {
        perror("Error in select");
        close(fd);
        return 1;
    } else if (ready == 0) {
        printf("Timeout: No data available in the serial port\n");
    } else {
        printf("Serial port is ready for reading\n");

        // 读取串口数据
        char buffer[255];
        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read == -1) {
            perror("Error reading from serial port");
            close(fd);
            return 1;
        }

        buffer[bytes_read] = '\0';
        printf("Received data: %s\n", buffer);
    }

    // 关闭串口
    close(fd);

    return 0;
}