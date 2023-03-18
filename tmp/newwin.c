#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main()
{
    int a[4][4] = {0};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            srand((unsigned int)time(NULL) + rand());
            a[i][j] = rand() % 9 ? 2 : 4;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d\t", a[i][j]);
        }
        putchar(10);
    }
}