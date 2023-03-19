#include "input.h"
#include <curses.h>
extern WINDOW *win_game;
int get_user_input()
{
    keypad(win_game, TRUE);
    char ch = wgetch(win_game);
    keypad(win_game, FALSE);
    if (ch == 'q' || ch == 'Q')
    {
        return QUIT;
    }
    else if (ch == 'a' || ch == 'A' || ch == KEY_LEFT)
    {
        return LEFT;
    }
    else if (ch == 'w' || ch == 'W' || ch == KEY_UP)
    {
        return UP;
    }
    else if (ch == 's' || ch == 'S' || ch == KEY_DOWN)
    {
        return DOWN;
    }
    else if (ch == 'd' || ch == 'D' || ch == KEY_RIGHT)
    {
        return RIGHT;
    }
}
