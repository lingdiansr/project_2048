#include <curses.h>
#include <stdio.h>
#define SIZE 4
#define ROW SIZE
#define COL SIZE
int a[ROW][COL] = {
    0, 2, 0, 0,
    0, 0, 0, 2,
    0, 0, 0, 0,
    0, 4, 0, 2};
int main()
{
    initscr();            // 初始化字符窗口
    cbreak();             // crtl+c
    noecho();             // 不用回显
    keypad(stdscr, TRUE); // 使用功能键
    
    WINDOW *t_win = newwin(4, 26, 5, 5);
    int ch = wgetch(t_win);
    waddch(t_win, (char)SIZE);
    if (ch=='8')
    {
        #ifdef SIZE
        #undef SIZE
        #define SIZE 8
        #endif 
    }
    waddch(t_win, (char)SIZE);
    while (1)
    {
        /* code */
    }
    
}