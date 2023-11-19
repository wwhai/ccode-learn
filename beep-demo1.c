#include <windows.h>

int main()
{
    for (size_t i = 0; i < 1000; i++)
    {
        Beep(i, 100);
    }

    return 0;
}
