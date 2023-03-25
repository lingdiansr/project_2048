#include <stdlib.h>
#include <time.h>
#include <curses.h>

#include "input.h"
#include "screen.h"

struct empty_pos
{
    int x;
    int y;
};

int ROW;
int COL;

WINDOW *win_game;
WINDOW *win_score;
int **matrix;
struct empty_pos *empty_sqe;

unsigned long long socre = 0;   //用于记录得分

void init_matrix()  //创建窗格
{
    // 申请行内存
    matrix = (int **)malloc(ROW * sizeof(int *));

    // 申请列内存
    for (int i = 0; i < ROW; i++)
    {
        matrix[i] = (int *)malloc(COL * sizeof(int));
    }

    // 初始化
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            matrix[i][j] = 0;
        }
    }
    empty_sqe = (struct empty_pos *)malloc(sizeof(struct empty_pos) * ROW * COL);
}
void free_matrix() // 释放内存
{
    for (int i = 0; i < ROW; i++)
    {
        free(matrix[i]);

    }
    free(matrix);
    free(empty_sqe);
    delwin(win_game);
    delwin(win_score);
}
void init_game_win(int width, int hight) //创建游戏主窗口
{
    win_game = newwin(width, hight, 8, 8);
    win_score = newwin(7, 10, 0, 0);
}
void get_score(int num) // 记录得分并输出
{
    socre += num;
    
}
void write_score(unsigned long long socre)//将得分记录写进本地文件
{
    FILE *fp;
    time_t now;
    time(&now);
    fp = fopen("score.txt", "w"); // 打开文件，如果不存在则创建一个新文件
    fprintf(fp, "时间：%s 分数：%llu",ctime(&now), socre); // 将分数写入文件
    fclose(fp);               // 关闭文件
}
int get_empty() // 获取空位置数量并把位置记录在sqe中
{
    for (int i = 0; i < ROW * COL; i++) // 位置初始化
    {
        empty_sqe[i].x = 0;
        empty_sqe[i].y = 0;
    }
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
void print_matrix() // 输出到指定窗口上
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            wmove(win_game, i * 2, j * 5);
            wprintw(win_game, "+-----+");
            wmove(win_game, i * 2 + 1, j * 5);
            if (matrix[i][j] == 0)
            {
                wprintw(win_game, "|     |");
            }
            else
            {
                wprintw(win_game, "|%4d |", matrix[i][j]);
            }
        }
        wmove(win_game, i * 2, COL * 5);
        wprintw(win_game, "+\n");
        wmove(win_game, i * 2 + 1, COL * 5);
        wprintw(win_game, "|\n");
    }
    for (j = 0; j < COL; j++)
    {
        wmove(win_game, ROW * 2, j * 5);
        wprintw(win_game, "+-----+");
    }
    wmove(win_game, ROW * 2, ROW * 5);
    wprintw(win_game, "+\n");
    wrefresh(win_game);
}
int random_num() // 随机生成2或4
{
    srand((unsigned int)time(NULL) + rand());
    return rand() % 9 ? 2 : 4;
}
void fill_rand_num() // 填入随机2/4
{
    srand(time(NULL));
    int n = get_empty();
    int pos = rand() % n;

    for (int i = 0; i < n; i++)
    {
        if (n == 1 || i + 1 == pos)
        {
            matrix[empty_sqe[i].x][empty_sqe[i].y] = random_num();
        }
    }
}
bool up_combine(int **block) // 向上合并
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
                if (block[k][i] == block[j][i] || (block[k][i] != 0 && block[k][i] != block[j][i]))
                {
                    break;
                }
            }
            if (block[k][i] == block[j][i]) // 相同合并
            {
                block[k][i] *= 2;
                block[j][i] = 0;
                flag = true;
            }
            else if (block[k][i] != 0 && block[k][i] != block[j][i]) // 不同移动到上一个位置
            {
                if (k < j - 1)                     //*
                {                                  // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    block[k + 1][i] = block[j][i]; //*
                    block[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                block[k][i] = block[j][i];
                block[j][i] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool down_combine(int **block) // 向下合并
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
                if (block[k][i] == block[j][i] || (block[k][i] != 0 && block[k][i] != block[j][i]))
                {
                    break;
                }
            }
            if (block[k][i] == block[j][i]) // 相同合并
            {
                block[k][i] *= 2;
                block[j][i] = 0;
                flag = true;
            }
            else if (block[k][i] != 0 && block[k][i] != block[j][i]) // 不同移动到上一个位置
            {
                if (k > j + 1)
                { // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    block[k - 1][i] = block[j][i];
                    block[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                block[k][i] = block[j][i];
                block[j][i] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool left_combine(int **block)
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < ROW; i++)
    {
        for (j = 1; j <= COL - 1; j++)
        {
            for (k = j - 1; k > 0; k--)
            {
                if (block[i][k] == block[i][j] || (block[i][k] != 0 && block[i][k] != block[i][j]))
                {
                    break;
                }
            }
            if (block[i][k] == block[i][j]) // 相同合并
            {
                block[i][k] *= 2;
                block[i][j] = 0;
                flag = true;
            }
            else if (block[i][k] != 0 && block[i][k] != block[i][j]) // 不同移动到上一个位置
            {
                if (k < j - 1)                     //*
                {                                  // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    block[i][k + 1] = block[i][j]; //*
                    block[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                block[i][k] = block[i][j];
                block[i][j] = 0;
                flag = true;
            }
            
        }
    }
    return flag;
}
bool right_combine(int **block)
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < ROW; i++)
    {
        for (j = COL - 2; j >= 0; j--)
        {
            for (k = j + 1; k < COL - 1; k++)
            {
                if ( block[i][k] ==  block[i][j] || ( block[i][k] != 0 &&  block[i][k] !=  block[i][j]))
                {
                    break;
                }
            }
            if ( block[i][k] ==  block[i][j]) // 相同合并
            {
                 block[i][k] *= 2;
                 block[i][j] = 0;
                flag = true;
            }
            else if ( block[i][k] != 0 &&  block[i][k] !=  block[i][j]) // 不同移动到上一个位置
            {
                if (k > j + 1)                       //*
                {                                    // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                     block[i][k - 1] =  block[i][j]; //*
                     block[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                 block[i][k] =  block[i][j];
                 block[i][j] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool judge_end()
{ // false表示游戏还能继续，true表示游戏结束

    int **m_backup;

    // 申请行内存
    m_backup = (int **)malloc(ROW * sizeof(int *));

    // 申请列内存
    for (int i = 0; i < ROW; i++)
    {
        m_backup[i] = (int *)malloc(COL * sizeof(int));
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            m_backup[i][j] = matrix[i][j];
        }
    }

    if (up_combine(m_backup) || down_combine(m_backup) || left_combine(m_backup) || right_combine(m_backup))
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
    init_matrix();
    curs_set(0);
    srand(time(NULL));
    init_game_win(100, 100);
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
            flag = left_combine(matrix);
            break;
        case RIGHT:
            flag = right_combine(matrix);
            break;
        case UP:
            flag = up_combine(matrix);
            break;
        case DOWN:
            flag = down_combine(matrix);
            break;
        case QUIT:
            return;
        default:
            break;
        }
        if (flag == TRUE)
        {
            fill_rand_num();
            print_matrix();
        }
        if (judge_end())
        {
            free_matrix();
            break;
        }
    }
}