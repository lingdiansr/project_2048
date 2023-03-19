#include <stdio.h>
#define SIZE 4
void printBoard(int board[SIZE][SIZE])
{
    int i, j;

    printf("+");
    for (j = 0; j < SIZE; j++)
    {
        printf("-----");
    }
    printf("+\n");

    for (i = 0; i < SIZE; i++)
    {
        printf("|");
        for (j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                char a = '.';
                printf("%5c", a);
            }
            else
            {
                printf("%5d", board[i][j]);
            }
        }
        printf("|\n\n");
    }

    printf("+");
    for (j = 0; j < SIZE; j++)
    {
        printf("-----");
    }
    printf("+\n");
}

int main()
{
    int a[SIZE][SIZE] = {0};
    printBoard(a);
    return 0;
}