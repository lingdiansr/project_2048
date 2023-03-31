#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <string.h>
// #include<sys

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
    char time[20];
} score_mark;
score_mark score_history[11];

int ROW;
int COL;

WINDOW *win_game;
WINDOW *win_score;

int **matrix;
struct empty_pos *empty_sqe;

unsigned long long score = 0; // 用于记录得分

void playsound()
{
    // WINDOW *tmp_sound_win = newwin(5, 5, 1000, 1000);
    char id_str[3];
    char cmd[80];
    sprintf(id_str, "%02d", rand() % 40 + 1);
    strcpy(cmd, "mplayer audio/");
    strcat(cmd, id_str);
    strcat(cmd, ".mp3 >/dev/null 2>&1 &");
    system(cmd);
    // delwin(tmp_sound_win);
}
// 函数名称：get_score_history
// 函数功能：从文件中读取历史得分记录并存储到数组中
// 参数：无
// 返回值：无
void get_score_history()
{
    // 定义文件名
    // 文件名为字符串类型的字符数组，可以在函数内部直接修改
    char filename[] = "history.txt";

    // 定义文件指针
    FILE *fp;

    // 定义字符串变量
    char line[40];

    // 打开文件
    fp = fopen(filename, "r");

    // 判断文件是否成功打开
    if (fp == NULL)
    {
        // 如果文件没有成功打开，则新建一个文件
        fp = fopen(filename, "w+");
        fclose(fp);
        return;
    }

    // 逐行读取文件内容
    int line_count = 0;
    while (fgets(line, 40, fp) != NULL && line_count < 10) // 只读取前10行
    {
        // 去掉换行符
        strtok(line, "\n");

        // 将读取到的内容按照格式存储到score_history数组中
        sscanf(line, "%d. score:%llu time:%s", &score_history[line_count].rand, &score_history[line_count].score, score_history[line_count].time);

        // 更新行数计数器
        line_count++;
    }

    // 关闭文件
    fclose(fp);
}
// 函数名：write_score
// 参数：无符号长整型 s，表示得分
// 返回值：无
// 功能：将得分记录写进本地文件
void write_score(unsigned long long s)
{
    FILE *fp;                                   // 文件指针
    time_t now;                                 // 时间变量
    time(&now);                                 // 获取当前时间
    score_mark temp;                            // 分数记录结构体
    score_history[10].score = s;                // 将得分存储到分数记录数组中
    score_history[10].rand = 11;                // 将随机数存储到分数记录数组中
    char *time_str = strtok(ctime(&now), "\n"); // 获取时间字符串
    for (int i = 0; i < strlen(time_str); i++)  // 将时间字符串中的空格替换为“-”
    {
        if (time_str[i] == ' ')
        {
            time_str[i] = '-';
        }
    }
    strcpy(score_history[10].time, time_str); // 将时间字符串存储到分数记录数组中
    for (int i = 0; i < 11; i++)              // 冒泡排序分数记录数组，按照分数从高到低排序
    {
        for (int j = 0; j < 10 - i; j++)
        {
            if (score_history[j].score < score_history[j + 1].score) // 如果前一个分数比后一个分数小，则交换两个分数记录
            {
                temp = score_history[j];
                score_history[j] = score_history[j + 1];
                score_history[j + 1] = temp;
            }
        }
    }

    fp = fopen("history.txt", "w+"); // 打开文件，如果不存在则创建一个新文件

    for (int i = 0; i < 10; i++) // 将排名前十的分数记录写入文件中
    {
        fprintf(fp, "%d. score:%llu time:%-s\n", i + 1, score_history[i].score, strtok(score_history[i].time, "\n"));
    }
    fclose(fp); // 关闭文件
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
    win_game = newwin(width, hight, 12, 8);
    win_score = newwin(10, 100, 0, 0);
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
    // 打印游戏窗格
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

    // 打印分数以及排行
    wclear(win_score);
    wprintw(win_score, "Current score:%d\n", score);
    for (int i = 0; i < 10; i++)
    {
        if (score_history[i].score != 0)
        {
            wprintw(win_score, "%d. score:%-8llu time:%-17s\n", score_history[i].rand, score_history[i].score, score_history[i].time);
        }
    }
    wrefresh(win_score);
}
void fill_rand_num()
{

    int n = get_empty();  // 获取空格子数量
    int pos = rand() % n; // 随机选取一个空格子

    // 遍历所有空格子
    for (int i = 0; i < n; i++)
    {
        // 如果只有一个空格子或者当前格子是随机选中的格子
        if (n == 1 || i == pos)
        {
            matrix[empty_sqe[i].x][empty_sqe[i].y] = rand() % 9 ? 2 : 4;
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
            if (block[k][i] == block[j][i]) // 先进行相同合并
            {
                score += block[k][i];
                block[k][i] *= 2;
                block[j][i] = 0;
                k++;
                flag = true;
            }
            else if (block[k][i] == 0) // 移动到空位置
            {
                block[k][i] = block[j][i];
                block[j][i] = 0;
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

            if (block[k][i] == block[j][i]) // 先进行相同合并
            {
                score += block[k][i];
                block[k][i] *= 2;
                block[j][i] = 0;
                k--;
                flag = true;
            }
            else if (block[k][i] == 0) // 移动到空位置
            {
                block[k][i] = block[j][i];
                block[j][i] = 0;
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
            if (block[i][k] == block[i][j]) // 相同合并
            {
                score += block[i][k];
                block[i][k] *= 2;
                block[i][j] = 0;
                k++;
                flag = true;
            }
            else if (block[i][k] == 0) // 下一个位置为空，移动到该位置
            {
                block[i][k] = block[i][j];
                block[i][j] = 0;
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
            if (block[i][k] == block[i][j]) // 相同合并
            {
                score += block[i][k];
                block[i][k] *= 2;
                block[i][j] = 0;
                k--;
                flag = true;
            }
            else if (block[i][k] == 0) // 下一个位置为空，移动到该位置
            {
                block[i][k] = block[i][j];
                block[i][j] = 0;
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
    bool flag;
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
        flag = false;
    }
    else
    {
        flag = true;
    }

    for (int i = 0; i < ROW; i++)
    {
        free(m_backup[i]);
    }
    free(m_backup); // 释放m_backup内存占用

    return flag;
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
            playsound();
        }
        if (judge_end())
        {
            write_score(score);
            free_matrix();
            break;
        }
    }
}