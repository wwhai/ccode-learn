#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_SIZE 2
#define HIDDEN_SIZE 3
#define OUTPUT_SIZE 1

// 定义神经网络结构
typedef struct
{
    double input[INPUT_SIZE];
    double hidden[HIDDEN_SIZE];
    double output[OUTPUT_SIZE];
    double weights_ih[INPUT_SIZE][HIDDEN_SIZE];
    double weights_ho[HIDDEN_SIZE][OUTPUT_SIZE];
} NeuralNetwork;

// 初始化神经网络
void initialize(NeuralNetwork *network)
{
    // 初始化连接权重
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        for (int j = 0; j < HIDDEN_SIZE; j++)
        {
            network->weights_ih[i][j] = ((double)rand() / RAND_MAX) * 2 - 1; // 范围在 -1 到 1 之间
        }
    }

    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        for (int j = 0; j < OUTPUT_SIZE; j++)
        {
            network->weights_ho[i][j] = ((double)rand() / RAND_MAX) * 2 - 1; // 范围在 -1 到 1 之间
        }
    }
}
double sigmoid(double sum)
{
    return 1 / (1 + exp(-sum));
}
// 前向传播
void forwardPropagation(NeuralNetwork *network)
{
    // 计算隐藏层
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < INPUT_SIZE; j++)
        {
            sum += network->input[j] * network->weights_ih[j][i];
        }
        network->hidden[i] = sigmoid(-sum);
    }

    // 计算输出层
    for (int i = 0; i < OUTPUT_SIZE; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < HIDDEN_SIZE; j++)
        {
            sum += network->hidden[j] * network->weights_ho[j][i];
        }
        network->output[i] = sigmoid(sum); // 使用Sigmoid作为激活函数
    }
}
// gcc ann-1.c -o anndemo -lm
int main()
{
    // 创建神经网络实例
    NeuralNetwork network;

    // 初始化神经网络
    initialize(&network);

    // 设置输入数据
    network.input[0] = 0.5;
    network.input[1] = 0.7;

    // 前向传播
    forwardPropagation(&network);

    // 打印输出结果
    printf("Output: %lf\n", network.output[0]);

    return 0;
}