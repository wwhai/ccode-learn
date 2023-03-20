#include <stdio.h>
#include <stdlib.h>

void gen_menu(int level, int maxLevel)
{
    if (level > maxLevel)
    {
        return;
    }
    for (int i = 0; i < level; i++)
    {
        printf("--");
    }
    printf("Level %d Menu\n", level);
    gen_menu(level + 1, maxLevel);
}

int main()
{
    int maxLevel = 10;
    gen_menu(1, maxLevel);
    return 0;
}
