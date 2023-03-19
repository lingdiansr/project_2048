#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include "input.h"
#include "screen.h"

#define SIZE 4
#define ROW SIZE
#define COL SIZE

WINDOW *win_game;
unsigned long long socre = 0;
int matrix[ROW][COL] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0};
struct empty_pos
{
    int x;
    int y;
} empty_sqe[ROW * COL]; // 记录空位置坐标

void init_game_win(int width, int hight)
{
    win_game = newwin(width, hight, 0, 0);
}
void get_score(int num)
{
    socre += num;
}
void empty_init() // 位置初始化
{
    for (int i = 0; i < ROW * COL; i++)
    {
        empty_sqe[i].x = 0;
        empty_sqe[i].y = 0;
    }
}
void bor() // 随机化matrix
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            srand((unsigned int)time(NULL) + rand());
            matrix[i][j] = rand() % 9 ? 2 : 4;
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
            if (matrix[i][j] == 0)
            {
                empty_sqe[n].x = i;
                empty_sqe[n].y = j;
                n++;
            }
        }
    }
    return n;
}
void print_matrix() // 输出到屏幕上
{
    // draw_grid();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            wmove(win_game,i, 6 * j);
            char buf[6];
            sprintf(buf, "%4d", matrix[i][j]);
            waddstr(win_game, buf);
        }
    }
    wrefresh(win_game);
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
            matrix[empty_sqe[i].x][empty_sqe[i].y] = random_num();
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
                if (matrix[k][i] == matrix[j][i] || (matrix[k][i] != 0 && matrix[k][i] != matrix[j][i]))
                {
                    break;
                }
            }
            if (matrix[k][i] == matrix[j][i]) // 相同合并
            {
                matrix[k][i] *= 2;
                get_score(matrix[j][i]);
                matrix[j][i] = 0;
                flag = true;
            }
            else if (matrix[k][i] != 0 && matrix[k][i] != matrix[j][i]) // 不同移动到上一个位置
            {
                if (k < j - 1)                       //*
                {                                    // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    matrix[k + 1][i] = matrix[j][i]; //*
                    get_score(matrix[j][i]);
                    matrix[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                matrix[k][i] = matrix[j][i];
                matrix[j][i] = 0;
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
                if (matrix[k][i] == matrix[j][i] || (matrix[k][i] != 0 && matrix[k][i] != matrix[j][i]))
                {
                    break;
                }
            }
            if (matrix[k][i] == matrix[j][i]) // 相同合并
            {
                matrix[k][i] *= 2;
                matrix[j][i] = 0;
                flag = true;
            }
            else if (matrix[k][i] != 0 && matrix[k][i] != matrix[j][i]) // 不同移动到上一个位置
            {
                if (k > j + 1)
                { // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    matrix[k - 1][i] = matrix[j][i];
                    matrix[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                matrix[k][i] = matrix[j][i];
                matrix[j][i] = 0;
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
                if (matrix[i][k] == matrix[i][j] || (matrix[i][k] != 0 && matrix[i][k] != matrix[i][j]))
                {
                    break;
                }
            }
            if (matrix[i][k] == matrix[i][j]) // 相同合并
            {
                matrix[i][k] *= 2;
                matrix[i][j] = 0;
                flag = true;
            }
            else if (matrix[i][k] != 0 && matrix[i][k] != matrix[i][j]) // 不同移动到上一个位置
            {
                if (k < j - 1)                       //*
                {                                    // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    matrix[i][k + 1] = matrix[i][j]; //*
                    matrix[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                matrix[i][k] = matrix[i][j];
                matrix[i][j] = 0;
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
                if (matrix[i][k] == matrix[i][j] || (matrix[i][k] != 0 && matrix[i][k] != matrix[i][j]))
                {
                    break;
                }
            }
            if (matrix[i][k] == matrix[i][j]) // 相同合并
            {
                matrix[i][k] *= 2;
                matrix[i][j] = 0;
                flag = true;
            }
            else if (matrix[i][k] != 0 && matrix[i][k] != matrix[i][j]) // 不同移动到上一个位置
            {
                if (k > j + 1)                       //*
                {                                    // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    matrix[i][k - 1] = matrix[i][j]; //*
                    matrix[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                matrix[i][k] = matrix[i][j];
                matrix[i][j] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool judge_end()
{ // false表示游戏还能继续，true表示游戏结束
    int m_backup[ROW][COL] = {0};
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            m_backup[i][j] = matrix[i][j];
        }
    }
    if (up_combine() || down_combine() || left_combine() || right_combine)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void game_2048()
{
    srand(time(NULL));
    // init_game_win(, 4, 26);
    init_game_win(4, 24);
    socre = 0;
    fill_rand_num();
    fill_rand_num();
    print_matrix();
    while (1)
    {
        bool flag = false;
        
        switch (get_user_input())
        {
        case LEFT:
            flag = left_combine();
            break;
        case RIGHT:
            flag = right_combine();
            break;
        case UP:
            flag = up_combine();
            break;
        case DOWN:
            flag = down_combine();
            break;
        case QUIT:
            return;
        default:
            break;
        }
        if (flag)
        {

            fill_rand_num();
            print_matrix();
        }
        if (judge_end())
        {
            
            break;
        }
        //wrefresh(win_game);
    }
}