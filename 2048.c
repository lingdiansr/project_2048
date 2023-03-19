#include <stdlib.h>
#include <time.h>
#include <curses.h>
WINDOW *win_game;
const int ROW = 4;
const int COL = 4;
int box[ROW][COL] = {0};
struct empty_pos
{
    int x;
    int y;
} empty_sqe[ROW * COL]; // 记录空位置坐标

void init_game_win(int width,int hight)
{
    win_game = newwin(width, hight, 0, 0);
}
void empty_init()       // 位置初始化
{
    for (int i = 0; i < ROW * COL; i++)
    {
        empty_sqe[i].x = 0;
        empty_sqe[i].y = 0;
    }
}
void bor() // 随机化box
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            srand((unsigned int)time(NULL) + rand());
            box[i][j] = rand() % 9 ? 2 : 4;
        }
    }
}
int get_empty() // 获取空位置数量并把位置记录在sqe中
{
    empty_init();
    int n = 0;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (box[i][j] == 0)
            {
                empty_sqe[n].x = i;
                empty_sqe[n].y = j;
                n++;
            }
        }
    }
    return n;
}
void print_box() // 输出到屏幕上
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d\t", box[i][j]);
        }
        printf("\n");
    }
}
int random_num() // 随机生成2或4
{
    srand((unsigned int)time(NULL) + rand());
    return rand() % (rand() % 10) ? 2 : 4;
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
            box[empty_sqe[i].x][empty_sqe[i].y] = random_num();
        }
    }
}
bool up_combine() // 向上合并
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < COL; i++)
    {
        for (j = 1; j <= ROW - 1; j++) //*
        {
            for (k = j - 1; k > 0; k--) //*
            {
                // 寻找合并位置
                // 1.上方经过或不经过空位有相同数字，合并到该位置
                // 2.上方有不同数字时，合并到上一个位置
                // 3.上方全为空，移动到该位置
                // if中为1. 2. 这两种情况需要提前跳出循环得到位置
                if (box[k][i] == box[j][i] || (box[k][i] != 0 && box[k][i] != box[j][i]))
                {
                    break;
                }
            }
            if (box[k][i] == box[j][i]) // 相同合并
            {
                box[k][i] *= 2;
                box[j][i] = 0;
                flag = true;
            }
            else if (box[k][i] != 0 && box[k][i] != box[j][i]) // 不同移动到上一个位置
            {
                if (k < j - 1)                 //*
                {                              // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[k + 1][i] = box[j][i]; //*
                    box[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[k][i] = box[j][i];
                box[j][i] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool down_combine() // 向下合并
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < COL; i++)
    {
        for (j = ROW - 2; j >= 0; j--)
        {
            for (k = j + 1; k < ROW - 1; k++)
            {
                // 寻找合并位置
                // 1.下方经过或不经过空位有相同数字，合并到该位置
                // 2.下方有不同数字时，合并到上一个位置
                // 3.下方全为空，移动到该位置
                // if中为1. 2. 这两种情况需要提前跳出循环得到位置
                if (box[k][i] == box[j][i] || (box[k][i] != 0 && box[k][i] != box[j][i]))
                {
                    break;
                }
            }
            if (box[k][i] == box[j][i]) // 相同合并
            {
                box[k][i] *= 2;
                box[j][i] = 0;
                flag = true;
            }
            else if (box[k][i] != 0 && box[k][i] != box[j][i]) // 不同移动到上一个位置
            {
                if (k > j + 1)
                { // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[k - 1][i] = box[j][i];
                    box[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[k][i] = box[j][i];
                box[j][i] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool left_combine()
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < ROW; i++)
    {
        for (j = 1; j <= COL - 1; j++)
        {
            for (k = j - 1; k > 0; k--)
            {
                if (box[i][k] == box[i][j] || (box[i][k] != 0 && box[i][k] != box[i][j]))
                {
                    break;
                }
            }
            if (box[i][k] == box[i][j]) // 相同合并
            {
                box[i][k] *= 2;
                box[i][j] = 0;
                flag = true;
            }
            else if (box[i][k] != 0 && box[i][k] != box[i][j]) // 不同移动到上一个位置
            {
                if (k < j - 1)                 //*
                {                              // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[i][k + 1] = box[i][j]; //*
                    box[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[i][k] = box[i][j];
                box[i][j] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool right_combine()
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < ROW; i++)
    {
        for (j = COL - 2; j >= 0; j--)
        {
            for (k = j + 1; k < COL - 1; k++)
            {
                if (box[i][k] == box[i][j] || (box[i][k] != 0 && box[i][k] != box[i][j]))
                {
                    break;
                }
            }
            if (box[i][k] == box[i][j]) // 相同合并
            {
                box[i][k] *= 2;
                box[i][j] = 0;
                flag = true;
            }
            else if (box[i][k] != 0 && box[i][k] != box[i][j]) // 不同移动到上一个位置
            {
                if (k > j + 1)                 //*
                {                              // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[i][k - 1] = box[i][j]; //*
                    box[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[i][k] = box[i][j];
                box[i][j] = 0;
                flag = true;
            }
        }
    }
    return flag;
}

void game_2048()
{
    srand(time(NULL));
    init_game_win(, 4, 26);
}