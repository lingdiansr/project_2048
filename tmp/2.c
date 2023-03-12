#include <stdio.h>
int main()
{
    int x, y, z;
    for (x = 1; x < 20; x++)
    {
        for (y = 1; y < 30; y++)
        {
            z = 100 - x - y;
            if (z > 1 && (100-5*x-3*y)*3+x+y==100)
            {
                printf("%d %d %d \n", x, y,z);
            }
        }
    }
}