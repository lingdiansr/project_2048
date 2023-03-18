#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <curses.h>
#define ROW 4
#define COL 4
int a[ROW][COL] = {
    0, 2, 0, 0,
    0, 0, 0, 2,
    0, 0, 0, 0,
    0, 4, 0, 2};
struct empty_pos
{
    int x;
    int y;
} empty_sqe[ROW * COL]; // 记录空位置坐标
void empty_init()       // 位置初始化
{
    for (int i = 0; i < ROW * COL; i++)
    {
        empty_sqe[i].x = 0;
        empty_sqe[i].y = 0;
    }
}
void bor() // 随机化
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            srand((unsigned int)time(NULL) + rand());
            a[i][j] = rand() % 9 ? 2 : 4;
        }
    }
}
int get_empty() // 获取空位置数量并把位置记录在sqe中
{
    int n = 0;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j] == 0)
            {
                n++;
                empty_sqe[n - 1].x = i;
                empty_sqe[n - 1].y = j;
            }
        }
    }
    return n;
}
void print_scr() // 输出到屏幕上
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }
}
int random_num() // 随机生成2或4
{
    srand(time(NULL));
    return (rand() % 2 + 1) * 2;
}
void fill_rand_num() // 填入随机2/4
{
    srand(time(NULL));
    int n = get_empty();
    int pos = rand() % n;
    for (int i = 0; i < n; i++)
    {
        if (i + 1 == pos)
        {
            a[empty_sqe[i].x][empty_sqe[i].y] = random_num();
        }
    }
}
void down_combine() // 向下合并
{
    for (int i = ROW - 1; i > 0; i--)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j] == a[i - 1][j]) // 如果上面的与下面的相同，合并，上面置零
            {
                a[i][j] *= 2;
                a[i - 1][j] = 0;
            }
            else if (a[i][j] == 0 && a[i - 1][j] != 0) // 如果下面为空，那么上面的下移
            {
                for (int k = i; k < 4; k++)
                {
                    if (a[k][j] != 0)
                    {
                        a[k - 1][j] = a[i - 1][j];
                        a[i - 1][j] = 0;
                    }
                }
            }
        }
    }
}
void init_win()
{
}
int main()
{
    // WINDOW *win_game;

    print_scr();
    putchar(10);
    down_combine();
    print_scr();
    putchar(10);
    fill_rand_num();
    print_scr();
    return 0;
}