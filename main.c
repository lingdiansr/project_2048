#include "2048.h"
#include "input.h"
#include "screen.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void random_num()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", (rand() % 2 + 1) * 2);
    }
}
int main()
{
    
}
