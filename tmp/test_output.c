#include <curses.h>
#include <stdio.h>
#define ROW 4
#define COL 4
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
    char b[8];
    WINDOW *t_win = newwin(4, 26, 5, 5);
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            wmove(t_win, i, 6 * j);
            sprintf(b, "%5d", a[i][j]);
            waddstr(t_win, b);
        }
    }
    wrefresh(t_win);
}