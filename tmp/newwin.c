#include<stdio.h>
#include<curses.h>

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr,TRUE);

    newwin(4, 4, 0, 0);
    while (1)
    {
        /* code */
    }
    return 0;
}