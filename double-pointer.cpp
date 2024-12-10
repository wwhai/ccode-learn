#include <stdio.h>
#include <stdlib.h>
void allocateMemory1(int *ptr)
{
    ptr = new int;
}
void allocateMemory2(int **ptr)
{
    *ptr = new int;
}
int main()
{
    {
        int *myPtr = nullptr;
        allocateMemory2(&myPtr);
        printf("allocateMemory2 = %p\n", myPtr);
        delete myPtr;
    }
    {
        int *myPtr = nullptr;
        allocateMemory1(myPtr);
        printf("allocateMemory1 = %p\n", myPtr);
    }
    return 0;
}
