#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>

int main()
{
    struct pollfd fds[2];
    int ret;

    // 监视 stdin 的可读状态
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    // 监视 stdout 的可写状态
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    // 轮询等待文件描述符的状态变化
    while ((ret = poll(fds, 2, -1)) != -1)
    {
        // 如果 stdin 可读，读取并输出数据
        if (fds[0].revents & POLLIN)
        {
            char buf[1024];
            int len = read(STDIN_FILENO, buf, sizeof(buf));
            if (len > 0)
            {
                write(STDOUT_FILENO, "stdin: ", 7);
                write(STDOUT_FILENO, buf, len);
            }
        }

        // 如果 stdout 可写，输出数据
        if (fds[1].revents & POLLOUT)
        {
            write(STDOUT_FILENO, "stdout: test\n", 13);
        }
    }

    return 0;
}