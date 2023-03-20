#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    struct termios tty;
    char buf[256];
    int n;

    // 打开串口设备文件
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }

    // 配置串口参数
    memset(&tty, 0, sizeof(tty));
    tty.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tcsetattr(fd, TCSANOW, &tty);

    // 写入数据
    n = write(fd, "Hello, world!", 13);
    if (n < 0)
    {
        perror("write");
        exit(1);
    }

    // 读取数据
    while (1)
    {
        n = read(fd, buf, sizeof(buf));
        if (n < 0)
        {
            perror("read");
            exit(1);
        }
        if (n > 0)
        {
            buf[n] = '\0';
            printf("Received: %s\n", buf);
            break;
        }
        usleep(1000);
    }

    // 关闭串口设备文件
    close(fd);

    return 0;
}