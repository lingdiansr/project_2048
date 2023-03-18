#include "screen.h"
#include <curses.h>

void open_screen()
{
    initscr();            // 初始化字符窗口
    cbreak();             // crtl+c
    noecho();             // 不用回显
    keypad(stdscr, TRUE); // 使用功能键
    WINDOW *win = newwin(16, 32, 5, 5);
}
void print_start()
{
    printf("欢迎游玩2048");
}
void close_screen()
{
    endwin(); // 关闭窗口
}