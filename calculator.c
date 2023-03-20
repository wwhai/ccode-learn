#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAX_EXPRESSION_SIZE 100

typedef enum
{
    lparen,
    rparen,
    plus,
    minus,
    times,
    divide,
    mod,
    eos,
    operand
} precedence;

int isp[] = {0, 19, 12, 12, 13, 13, 13, 0};
int icp[] = {20, 19, 12, 12, 13, 13, 13, 0};

precedence stack[MAX_STACK_SIZE];
int top = -1;
char expression[MAX_EXPRESSION_SIZE];
int pos = 0;

void push(precedence token)
{
    if (top >= MAX_STACK_SIZE - 1)
    {
        printf("Stack overflow\n");
        exit(1);
    }
    stack[++top] = token;
}

precedence pop()
{
    if (top == -1)
    {
        printf("Stack underflow\n");
        exit(1);
    }
    return stack[top--];
}

precedence getToken(char *symbol, int *n)
{
    *symbol = expression[(*n)++];
    switch (*symbol)
    {
    case '(':
        return lparen;
    case ')':
        return rparen;
    case '+':
        return plus;
    case '-':
        return minus;
    case '*':
        return times;
    case '/':
        return divide;
    case '%':
        return mod;
    case '\0':
        return eos;
    default:
        return operand;
    }
}

void printToken(precedence token)
{
    switch (token)
    {
    case lparen:
        printf("(");
        break;
    case rparen:
        printf(")");
        break;
    case plus:
        printf("+");
        break;
    case minus:
        printf("-");
        break;
    case times:
        printf("*");
        break;
    case divide:
        printf("/");
        break;
    case mod:
        printf("%%");
        break;
    default:
        break;
    }
}

void printStack()
{
    for (int i = 0; i <= top; i++)
    {
        printToken(stack[i]);
    }
    printf("\n");
}

void postfix()
{
    char symbol;
    precedence token;
    top = 0;
    stack[0] = eos;
    for (token = getToken(&symbol, &pos); token != eos; token = getToken(&symbol, &pos))
    {
        if (token == operand)
        {
            printf("%c", symbol);
        }
        else if (token == rparen)
        {
            while (stack[top] != lparen)
            {
                printToken(pop());
            }
            pop();
        }
        else
        {
            while (isp[stack[top]] >= icp[token])
            {
                printToken(pop());
            }
            push(token);
        }
    }
    while ((token = pop()) != eos)
    {
        printToken(token);
    }
    printf("\n");
}

int evaluate()
{
    int op1, op2;
    char symbol;
    precedence token;
    top = 0;
    stack[0] = eos;
    for (token = getToken(&symbol, &pos); token != eos; token = getToken(&symbol, &pos))
    {
        if (token == operand)
        {
            push(symbol - '0');
        }
        else
        {
            op2 = pop();
            op1 = pop();
            switch (token)
            {
            case plus:
                push(op1 + op2);
                break;
            case minus:
                push(op1 - op2);
                break;
            case times:
                push(op1 * op2);
                break;
            case divide:
                push(op1 / op2);
                break;
            case mod:
                push(op1 % op2);
                break;
            default:
                break;
            }
        }
    }
    return pop();
}

int main()
{
    printf("Enter an expression: ");
    scanf("%s", expression);
    postfix();
    pos = 0;
    printf("Result: %d\n", evaluate());
    return 0;
}