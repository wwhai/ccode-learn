#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
// 定义栈结构
#define MAX_STACK_SIZE 100

typedef struct
{
    int top;
    double items[MAX_STACK_SIZE];
} Stack;

// 栈操作：入栈
void push(Stack *stack, double value)
{
    if (stack->top < MAX_STACK_SIZE - 1)
    {
        stack->top++;
        stack->items[stack->top] = value;
    }
    else
    {
        fprintf(stderr, "Stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

// 栈操作：出栈
double pop(Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top--];
    }
    else
    {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
}

// 栈操作：获取栈顶元素
double peek(const Stack *stack)
{
    if (stack->top >= 0)
    {
        return stack->items[stack->top];
    }
    else
    {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
}

// 判断运算符优先级
int precedence(char operator)
{
    if (operator== '+' || operator== '-')
    {
        return 1;
    }
    else if (operator== '*' || operator== '/')
    {
        return 2;
    }
    else
    {
        return 0; // 默认优先级为 0
    }
}

// 将中缀表达式转换为后缀表达式
void infixToPostfix(const char *infix, char *postfix)
{
    Stack operatorStack;
    operatorStack.top = -1;

    int i = 0, j = 0;
    while (infix[i] != '\0')
    {
        if (isdigit(infix[i]) || infix[i] == '.')
        {
            // 处理数值
            while (isdigit(infix[i]) || infix[i] == '.')
            {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; // 用空格分隔数值
        }
        else if (isalpha(infix[i]))
        {
            // 处理函数名
            char functionName[10];
            int k = 0;
            while (isalpha(infix[i]))
            {
                functionName[k++] = infix[i++];
            }
            functionName[k] = '\0';
            i--;

            // 如果是支持的函数，压入栈
            if (strcmp(functionName, "sin") == 0)
            {
                push(&operatorStack, 's');
            }
            else if (strcmp(functionName, "cos") == 0)
            {
                push(&operatorStack, 'c');
            }
            else if (strcmp(functionName, "tan") == 0)
            {
                push(&operatorStack, 't');
            }
            else
            {
                fprintf(stderr, "Unsupported function: %s\n", functionName);
                exit(EXIT_FAILURE);
            }
        }
        else if (infix[i] == '(')
        {
            // 左括号直接入栈
            push(&operatorStack, infix[i]);
        }
        else if (infix[i] == ')')
        {
            // 弹出栈中左括号之前的运算符，并输出到后缀表达式
            while (peek(&operatorStack) != '(')
            {
                postfix[j++] = pop(&operatorStack);
                postfix[j++] = ' '; // 用空格分隔运算符
            }
            pop(&operatorStack); // 弹出 '('
        }
        else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')
        {
            // 处理运算符
            while (precedence(infix[i]) <= precedence(peek(&operatorStack)))
            {
                postfix[j++] = pop(&operatorStack);
                postfix[j++] = ' '; // 用空格分隔运算符
            }
            push(&operatorStack, infix[i]);
        }
        i++;
    }

    // 弹出栈中剩余的运算符，并输出到后缀表达式
    while (operatorStack.top >= 0)
    {
        postfix[j++] = pop(&operatorStack);
        postfix[j++] = ' '; // 用空格分隔运算符
    }

    postfix[j] = '\0'; // 在末尾添加字符串结束符
}

// 计算后缀表达式的值
double evaluatePostfix(const char *postfix)
{
    Stack operandStack;
    operandStack.top = -1;

    int i = 0;
    while (postfix[i] != '\0')
    {
        if (isdigit(postfix[i]) || postfix[i] == '.')
        {
            double operand = atof(&postfix[i]);
            push(&operandStack, operand);

            // 跳过当前数字
            while (isdigit(postfix[i]) || postfix[i] == '.')
            {
                i++;
            }
        }
        else if (isalpha(postfix[i]))
        {
            // 处理函数
            char functionName = pop(&operandStack);
            double operand = pop(&operandStack);
            double result;

            switch (functionName)
            {
            case 's':
                result = sin(operand);
                break;
            case 'c':
                result = cos(operand);
                break;
            case 't':
                result = tan(operand);
                break;
            default:
                fprintf(stderr, "Error: Unsupported function\n");
                exit(EXIT_FAILURE);
            }

            push(&operandStack, result);
        }
        else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/')
        {
            double operand2 = pop(&operandStack);
            double operand1 = pop(&operandStack);
            double result;

            switch (postfix[i])
            {
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                if (operand2 != 0)
                {
                    result = operand1 / operand2;
                }
                else
                {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }

            push(&operandStack, result);
        }

        i++;
    }

    return peek(&operandStack);
}

int main()
{
    char infixExpression[] = "3 + sin(45) - 6";
    char postfixExpression[100];

    // 将中缀表达式转换为后缀表达式
    infixToPostfix(infixExpression, postfixExpression);

    printf("Infix Expression: %s\n", infixExpression);
    printf("Postfix Expression: %s\n", postfixExpression);

    // 计算后缀表达式的值
    double result = evaluatePostfix(postfixExpression);
    printf("Result: %.2f\n", result);

    return 0;
}
