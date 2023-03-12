#include "2048.h"
#include "input.h"
#include "screen.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>




int main()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", (rand()%2+1)*2);
    }
}
