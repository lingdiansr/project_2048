#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <string.h>

#include "input.h"
#include "screen.h"

struct empty_pos
{
    int x;
    int y;
};
typedef struct
{
    int rand;                 // 存储排名
    unsigned long long score; // 存储得分，8位数
    char time[25];
} score_mark;
score_mark score_history[11];

int ROW;
int COL;

WINDOW *win_game;
WINDOW *win_score;

int **matrix;
struct empty_pos *empty_sqe;

unsigned long long score = 0; // 用于记录得分

void get_score_history()
{
    char filename[] = "history.txt";
    FILE *fp;
    char line[40];

    fp = fopen(filename, "r"); // 打开文件
    if (fp == NULL)
    {
        // 文件没有成功打开时，新建一个文件
        fp = fopen(filename, "w+");
        fclose(fp);
        return;
    }

    // 逐行读取文件内容
    int line_count = 0;
    while (fgets(line, 40, fp) != NULL && line_count < 10) // 修改条件，只读取前10行
    {
        strtok(line, "\n"); // 去掉换行符

        // 将读取到的内容按照格式存储到score_history数组中
        sscanf(line, "%d. score:%llu time:%s", &score_history[line_count].rand, &score_history[line_count].score, score_history[line_count].time);

        line_count++; // 更新行数计数器
    }
    fclose(fp); // 关闭文件
}
void write_score(unsigned long long s) // 将得分记录写进本地文件
{
    FILE *fp;
    time_t now;
    time(&now);
    score_mark temp;
    score_history[10].score = s;
    score_history[10].rand = 11;
    char *time_str = strtok(ctime(&now), "\n");
    for (int i = 0; i < strlen(time_str); i++)
    {
        if (time_str[i] == ' ')
        {
            time_str[i] = '-';
        }
    }
    strcpy(score_history[10].time, time_str);
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 10 - i; j++)
        {
            if (score_history[j].score < score_history[j + 1].score) // 修改比较条件
            {
                temp = score_history[j];
                score_history[j] = score_history[j + 1];
                score_history[j + 1] = temp;
            }
        }
    }

    fp = fopen("history.txt", "w+"); // 打开文件，如果不存在则创建一个新文件

    for (int i = 0; i < 10; i++)
    {
        printf("%d. score:%llu time:%s\n", i + 1, score_history[i].score, strtok(score_history[i].time, "\n"));
        fprintf(fp, "%d. score:%llu time:%-s\n", i + 1, score_history[i].score, strtok(score_history[i].time, "\n"));
    }
    fclose(fp);
}

void init_matrix() // 创建窗格
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
    refresh();
}
void init_game_win(int width, int hight) // 创建游戏主窗口
{
    win_game = newwin(width, hight, 8, 8);
    win_score = newwin(7, 10, 0, 0);
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
    bool flag = false;
    for (int i = 0; i < COL; i++)
    {
        int k = 0; // 记录下一个可以移动的位置
        for (int j = 1; j < COL; j++)
        {
            if (block[j][i] == 0) // 当前位置为空，跳过
                continue;
            if (block[k][i] == 0) // 下一个位置为空，移动到该位置
            {
                block[k][i] = block[j][i];
                block[j][i] = 0;
                flag = true;
            }
            else if (block[k][i] == block[j][i]) // 相同合并
            {
                block[k][i] *= 2;
                block[j][i] = 0;
                k++; // 合并后k位置不变
                flag = true;
            }
            else // 不同移动到上一个位置
            {
                k++;
                if (k != j) // 需要移动
                {
                    block[k][i] = block[j][i];
                    block[j][i] = 0;
                    flag = true;
                }
            }
        }
    }
    return flag;
}
bool down_combine(int **block) // 向下合并
{
    bool flag = false;
    for (int i = 0; i < COL; i++)
    {
        int k = ROW - 1; // 记录下一个可以移动的位置
        for (int j = ROW - 2; j >= 0; j--)
        {
            if (block[j][i] == 0) // 当前位置为空，跳过
                continue;
            if (block[k][i] == 0) // 下一个位置为空，移动到该位置
            {
                block[k][i] = block[j][i];
                block[j][i] = 0;
                flag = true;
            }
            else if (block[k][i] == block[j][i]) // 相同合并
            {
                block[k][i] *= 2;
                block[j][i] = 0;
                k--; // 合并后k位置不变
                flag = true;
            }
            else // 不同移动到上一个位置
            {
                k--;
                if (k != j) // 需要移动
                {
                    block[k][i] = block[j][i];
                    block[j][i] = 0;
                    flag = true;
                }
            }
        }
    }
    return flag;
}
bool left_combine(int **block)
{
    bool flag = false;
    for (int i = 0; i < ROW; i++)
    {
        int k = 0; // 记录下一个可以移动的位置
        for (int j = 1; j < COL; j++)
        {
            if (block[i][j] == 0) // 当前位置为空，跳过
                continue;
            if (block[i][k] == 0) // 下一个位置为空，移动到该位置
            {
                block[i][k] = block[i][j];
                block[i][j] = 0;
                flag = true;
            }
            else if (block[i][k] == block[i][j]) // 相同合并
            {
                block[i][k] *= 2;
                block[i][j] = 0;
                k++; // 合并后k位置不变
                flag = true;
            }
            else // 不同移动到上一个位置
            {
                k++;
                if (k != j) // 需要移动
                {
                    block[i][k] = block[i][j];
                    block[i][j] = 0;
                    flag = true;
                }
            }
        }
    }
    return flag;
}

// 优化思路：

// 精简循环：将三层循环合并为两层，同时在循环中判断当前位置是否为空，避免不必要的循环。
// 优化查找合并位置的算法：将查找合并位置的循环改为记录下一个可以移动的位置，大大降低了时间复杂度。
// 减少重复操作：合并后的位置不需要再次移动，通过记录下一个可以移动的位置来避免重复操作。
bool right_combine(int **block)
{
    bool flag = false;
    for (int i = 0; i < ROW; i++)
    {
        int k = COL - 1;
        for (int j = COL - 2; j >= 0; j--)
        {
            if (block[i][j] == 0) // 当前位置为空，跳过
                continue;
            if (block[i][k] == 0) // 下一个位置为空，移动到该位置
            {
                block[i][k] = block[i][j];
                block[i][j] = 0;
                flag = true;
            }
            else if (block[i][k] == block[i][j]) // 相同合并
            {
                block[i][k] *= 2;
                block[i][j] = 0;
                k--; // 合并后k位置不变
                flag = true;
            }
            else // 不同移动到上一个位置
            {
                k--;
                if (k != j) // 需要移动
                {
                    block[i][k] = block[i][j];
                    block[i][j] = 0;
                    flag = true;
                }
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
    score = 0;
    get_score_history(); // 各种初始化

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
            write_score(score);
            free_matrix();
            break;
        }
    }
}