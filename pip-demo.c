#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t pid1, pid2;
    char buffer[BUFFER_SIZE];

    if (argc < 2)
    {
        printf("Usage: %s command\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0)
    {                                   // 子进程1，执行命令
        close(pipefd[0]);               // 关闭管道的读端
        dup2(pipefd[1], STDOUT_FILENO); // 将标准输出重定向到管道的写端
        close(pipefd[1]);               // 关闭管道的写端
        execlp(argv[1], argv[1], NULL); // 执行命令
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else
    { // 父进程
        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0)
        {                     // 子进程2，读取命令输出
            close(pipefd[1]); // 关闭管道的写端
            while (read(pipefd[0], buffer, BUFFER_SIZE) > 0)
            {
                printf("%s", buffer); // 输出命令输出
            }
            close(pipefd[0]); // 关闭管道的读端
            exit(EXIT_SUCCESS);
        }
        else
        {                           // 父进程
            close(pipefd[0]);       // 关闭管道的读端
            close(pipefd[1]);       // 关闭管道的写端
            waitpid(pid1, NULL, 0); // 等待子进程1结束
            waitpid(pid2, NULL, 0); // 等待子进程2结束
            exit(EXIT_SUCCESS);
        }
    }
}