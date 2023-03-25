#include <curses.h>
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
    endwin(); // 关闭窗口
}
void begin_screen() // 创建一个主界面窗口
{
    int x0 = 4, y0 = 4;

    // initscr();
    // noecho();

    win_begin = newwin(100, 100, y0, x0);
    // draw_grid();
    curs_set(0);
    wmove(win_begin, 0, 5);
    waddstr(win_begin, "            WELCOME TO 2048         ");
    wmove(win_begin, 4, 4);
    waddstr(win_begin, "   1.Easy mode,8*8 size: enter E    ");
    wmove(win_begin, 8, 4);
    waddstr(win_begin, "2.Little easy mode,6*6 size: enter L");
    wmove(win_begin, 12, 4);
    waddstr(win_begin, "  3.Normal mode,4*4 size: enter N   ");

    wrefresh(win_begin);
    
    set_size();
    
}