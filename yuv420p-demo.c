#include <stdio.h>
#include <stdlib.h>

void fill_YUV420p(unsigned char *YUV420p, int width, int height)
{
    int i, j;
    // 填充Y分量
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            YUV420p[j * width + i] = (i + j) % 256; // 生成灰度条纹
        }
    }
    // 填充U分量
    for (j = 0; j < height / 2; j++)
    {
        for (i = 0; i < width / 2; i++)
        {
            YUV420p[width * height + j * width / 2 + i] = 128; // 中间色度值
        }
    }
    // 填充V分量
    for (j = 0; j < height / 2; j++)
    {
        for (i = 0; i < width / 2; i++)
        {
            YUV420p[width * height + width * height / 4 + j * width / 2 + i] = 128; // 中间色度值
        }
    }
}

int main()
{
    int width = 640;                   // 宽度
    int height = 480;                  // 高度
    int size = width * height * 3 / 2; // YUV420P帧的大小

    // 分配内存
    unsigned char *YUV420p = (unsigned char *)malloc(size);
    if (YUV420p == NULL)
    {
        fprintf(stderr, "Could not allocate memory for YUV420P frame.\n");
        return -1;
    }

    // 填充YUV420P帧
    fill_YUV420p(YUV420p, width, height);

    // 将YUV420P帧写入文件
    FILE *file = fopen("test.yuv", "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file for writing.\n");
        free(YUV420p);
        return -1;
    }

    fwrite(YUV420p, 1, size, file);
    fclose(file);

    // 释放内存
    free(YUV420p);

    printf("YUV420P frame generated and saved to 'test.yuv'.\n");

    return 0;
}
