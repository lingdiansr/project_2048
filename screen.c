#include "screen.h"
#include <curses.h>

WINDOW *open_screen()
{
    initscr();            // 初始化字符窗口
    cbreak();             // crtl+c
    noecho();             // 不用回显
    keypad(stdscr, TRUE); // 使用功能键
    WINDOW *win = newwin(16, 32, 5, 5);
    return win;
}
void print_start()
{
    printf()
}
void close_screen()
{
    endwin(); // 关闭窗口
}