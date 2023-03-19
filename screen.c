#include "screen.h"
#include <curses.h>

// 绘制窗格
void draw_grid(void)
{
    int m, n, x, y;
    clear(); // 将整个屏幕清除
    // 绘制横线 m行 n列
    for (m = 0; m < 9; m += 2)
    {
        for (n = 0; n < 21; n++)
        {
            move(m, n); // 将光标移动至 m,n 的位置.
            addch('-'); // 在当前光标位置输入单个字符，并将光标右移一位。
            refresh();  // 刷新屏幕,使绘制即时生效
        }
    }
    // 绘制竖线
    for (n = 0; n < 22; n += 5)
    {
        for (m = 1; m < 8; m++)
        {
            move(m, n);
            addch('|');
            refresh();
        }
    }
}
void begin_screen() // 创建一个主界面窗口
{
    initscr();
    noecho();
    WINDOW *win_begin = newwin(10, 10, 0, 0);
    mvwaddch(win_begin, 2, 2, "WELCOME 2048");
    wrefresh(win_begin);
    keypad(stdscr, TRUE);
    while (1)
        ;
}

void open_screen()
{
    initscr();            // 初始化字符窗口
    cbreak();             // ctrl+c结束游戏进程
    noecho();             // 不用回显
    keypad(stdscr, TRUE); // 使用功能键：上下左右
    newwin(4, 4, 100, 100);
    draw_grid();
    curs_set(0);
    while (1)
        ;
}

void close_screen()
{
    endwin(); // 关闭窗口
}
