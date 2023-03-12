#include <curses.h>

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    WINDOW *win = newwin(4,4, 0 ,0);
    waddstr(win, "hello world");
    wrefresh(win);
    mvwin(win, 10, 10);
    wrefresh(win);
    while (1)
        ;
    return 0;
}