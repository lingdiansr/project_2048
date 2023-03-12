#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void ran(int num, int size)
{
    int *a = (int *)malloc(sizeof(int) * num);
    int tmp;
    int i, j;
    srandom(time(NULL));
    a[0] = random() % size;
    for ( i = 1; i < num; i++)
    {
        tmp = random() % size;
        for ( j = 0; j < i; j++)
        {
            if (a[j] == tmp)
            {
                i--;
                break;
            }
        }
        if (j==i)
        {
            a[i] = tmp;
        }
        
    }
    for (int i =0; i < num; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}
int main()
{
    ran(6, 20);
    return 0;
}