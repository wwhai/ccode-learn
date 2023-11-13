#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 8192

static ucontext_t context[2];
static int current = 0;

void coroutine_1()
{
    printf("Coroutine 1: Start\n");

    while (1) {
        printf("Coroutine 1: Running\n");
        swapcontext(&context[1], &context[0]);
    }
}

void coroutine_2()
{
    printf("Coroutine 2: Start\n");

    while (1) {
        printf("Coroutine 2: Running\n");
        swapcontext(&context[0], &context[1]);
    }
}

int main()
{
    char stack[STACK_SIZE];

    getcontext(&context[0]);
    context[0].uc_stack.ss_sp = stack;
    context[0].uc_stack.ss_size = sizeof(stack);
    context[0].uc_link = &context[1];
    makecontext(&context[0], coroutine_1, 0);

    getcontext(&context[1]);
    context[1].uc_stack.ss_sp = malloc(STACK_SIZE);
    context[1].uc_stack.ss_size = STACK_SIZE;
    context[1].uc_link = &context[0];
    makecontext(&context[1], coroutine_2, 0);

    printf("Switching to Coroutine 1\n");
    swapcontext(&context[1], &context[0]);

    return 0;
}