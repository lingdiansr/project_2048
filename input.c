#include "input.h"
#include "screen.h"
#include "2048.h"
#include <curses.h>

extern WINDOW *win_game;
extern WINDOW *win_begin;
extern WINDOW *win_pattern;
extern WINDOW *win_close;
extern int ROW;
extern int COL;
int cond;
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
    keypad(win_close, true);
    int ch = wgetch(win_close);
    keypad(win_close, false);
    if (ch == 'q' || ch == 'Q')
    {
        return QUIT;
    }
    else if (ch == 'r' || ch == 'R')
    {

        wclear(win_close);
        delwin(win_close);

        refresh();
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
void pattern_input()
{
    keypad(win_pattern, true);
    int cha = wgetch(win_pattern);
    keypad(win_pattern, false);
    if (cha == 'c' || cha == 'C')
    {
        cond=5;
    }
    else if (cha == 'd' || cha == 'D')
    {
        cond= 6;
    }
}


int ra()
{
    int x;
    switch (cond)
    {
    case 5:
        x = rand() % 9 ? 2 : 4;
    case 6:
        x = (rand() % 2 + 1) * 64;
        return x;
    }
}