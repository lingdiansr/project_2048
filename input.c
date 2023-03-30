#include "input.h"
#include "screen.h"
#include "2048.h"
#include <curses.h>

extern WINDOW *win_game;
extern WINDOW *win_begin;
extern int ROW;
extern int COL;
void set_size()
{
    switch (begain_input())
    {
    case QUIT:
        close_screen();
        break;
    case EAS_MODE:
        ROW = 8;
        COL = 8;
        break;
    case LIT_MODE:
        ROW = 6;
        COL = 6;
        break;
    default:
        ROW = 4;
        COL = 4;
        break;
    }
}
int begain_input()
{
    keypad(win_begin, true);
    int ch = wgetch(win_begin);
    keypad(win_begin, false);
    if (ch == 'q' || ch == 'Q')
    {
        return QUIT;
    }
    else if (ch == 'e' || ch == 'E')
    {
        return EAS_MODE;
    }
    else if (ch == 'l' || ch == 'L')
    {
        return LIT_MODE;
    }
    else if (ch == 'n' || ch == 'N')
    {
        return NOR_MODE;
    }
}

int close_input()
{
    keypad(close_screen(), true);
    int ch = wgetch(close_screen());
    keypad(close_screen(), false);
    if (ch == 'q' || ch == 'Q')
    {
        return QUIT;
    }
    else if (ch == 'r' || ch == 'R')
    {
        game_2048();
    }

}
int get_user_input()
{
    keypad(win_game, TRUE);
    int ch = wgetch(win_game);
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
