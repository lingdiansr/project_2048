#include "screen.h"
#include <curses.h>
void open_screen()
{
    initscr();  //初始化字符窗口
    cbreak();   //crtl+c
    noecho();   //不用回显
    keypad(stdscr, TURE);   //使用功能键
}

void close_screen();
{
    endwin();   //关闭窗口
}