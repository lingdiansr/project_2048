#include <curses.h>
#include <string.h>
#include "input.h"
#include "2048.h"

// 绘制窗格
WINDOW *win_begin;

void open_screen()
{
    initscr();            // 初始化字符窗口
    cbreak();             // ctrl+c结束游戏进程
    noecho();             // 不用回显
    keypad(stdscr, TRUE); // 使用功能键：上下左右
                          // newwin(4, 4, 100, 100);
    // draw_grid();
    // curs_set(0);
    // while (1)
    ;
}

void close_screen()
{
    int x0 = 8, y0 = 8;
    int width = 50;
    curs_set(0);
    int restart_x = (width - strlen("1. restart: press the key R twice")) / 2;
    int native_chart_x = (width - strlen("2. open the score chart: press the key S twice")) / 2;
    int quit_x = (width - strlen("4.press Q to quit")) / 2;

    mvwprintw(win_begin, 4, restart_x, "1. restart: press the key R twice");
    mvwprintw(win_begin, 8, native_chart_x, "2. open the score chart: press the key S twice");
    mvwprintw(win_begin, 12, quit_x, "4.press Q to quit");

    wrefresh(win_begin);
    set_size();
    delwin(win_begin);
    refresh();

    endwin(); // 关闭窗口
}
void begin_screen() // 创建一个主界面窗口
{
    int x0 = 8, y0 = 8;
    int width = 50;

    win_begin = newwin(100, width, y0, x0);
    curs_set(0);

    // 计算居中位置并输出文字
    int welcome_x = (width - strlen("WELCOME TO 2048")) / 2;
    int easy_x = (width - strlen("1.Easy mode,8*8 size: press the key E twice")) / 2;
    int little_x = (width - strlen("2.Little easy mode,6*6 size: press the key L twice")) / 2;
    int normal_x = (width - strlen("3.Normal mode,4*4 size: press the key N twice")) / 2;
    int quit_x = (width - strlen("4.press Q to quit")) / 2;

    mvwprintw(win_begin, 0, welcome_x, "WELCOME TO 2048");
    mvwprintw(win_begin, 4, easy_x, "1.Easy mode,8*8 size: press the key E twice");
    mvwprintw(win_begin, 8, little_x, "2.Little easy mode,6*6 size: press the key L twice");
    mvwprintw(win_begin, 12, normal_x, "3.Normal mode,4*4 size: press the key N twice");
    mvwprintw(win_begin, 16, quit_x, "4.press Q to quit");

    wrefresh(win_begin);
    set_size();
    delwin(win_begin);
    refresh();
}