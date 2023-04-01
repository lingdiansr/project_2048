#include <curses.h>
#include <string.h>

#include "input.h"
#include "2048.h"
#include "main.h"

extern unsigned long long score;
// 绘制窗格
WINDOW *win_begin;
WINDOW *win_close;

void open_screen()
{
    initscr();            // 初始化字符窗口
    cbreak();             // ctrl+c结束游戏进程
    noecho();             // 不用回显
    keypad(stdscr, TRUE); // 使用功能键：上下左右
}
void close_screen()
{
    write_score(score); // 结束时写入得分
    int x0 = 8, y0 = 8;
    int width = 50;

    win_close = newwin(100, width, y0, x0);
    curs_set(0);

    char str_score[25] = {"Your score is :"};
    char char_score[10];
    sprintf(char_score, " %-llu", score);
    strcat(str_score, char_score); // 将得分写入字符串，用于在结束窗口展示

    // 以下整形用于计算字符串居中时的输出位置
    int over_x = (width - strlen("Game Over !!!")) / 2;
    int score_x = (width - strlen(str_score)) / 2;
    int restart_x = (width - strlen("1. restart: press the key R")) / 2;
    int quit_x = (width - strlen("2.press Q to quit")) / 2;

    // 输出
    mvwprintw(win_close, 0, over_x, "Game Over !!!");
    mvwprintw(win_close, 4, score_x, str_score);
    mvwprintw(win_close, 8, restart_x, "1. restart: press the key R");
    mvwprintw(win_close, 12, quit_x, "2.press Q to quit");

    wrefresh(win_close);

    switch (close_input())
    {
    case RESTART:
        main(); // 重新开始游戏
        break;
    default:
        break;
    }
    endwin();
}

void begin_screen() // 创建一个启动界面窗口
{
    int x0 = 8, y0 = 8;
    int width = 50;

    win_begin = newwin(100, width, y0, x0);
    curs_set(0);

    // 计算居中位置并输出文字
    int welcome_x = (width - strlen("WELCOME TO 2048")) / 2;
    int easy_x = (width - strlen("1.Easy mode,8*8 size: press the key E ")) / 2;
    int little_x = (width - strlen("2.Little easy mode,6*6 size: press the key L")) / 2;
    int normal_x = (width - strlen("3.Normal mode,4*4 size: press the key N")) / 2;
    int quit_x = (width - strlen("4.press Q to quit")) / 2;

    mvwprintw(win_begin, 0, welcome_x, "WELCOME TO 2048");
    mvwprintw(win_begin, 4, easy_x, "1.Easy mode,8*8 size: press the key E");
    mvwprintw(win_begin, 8, little_x, "2.Little easy mode,6*6 size: press the key L");
    mvwprintw(win_begin, 12, normal_x, "3.Normal mode,4*4 size: press the key N");
    mvwprintw(win_begin, 16, quit_x, "4.press Q to quit");

    wrefresh(win_begin);
    set_size(); // 获取输入，设定游戏网格大小
    endwin();
}