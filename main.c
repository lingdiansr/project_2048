#include "2048.h"
#include "input.h"
#include "screen.h"

#include <stdio.h>
#include <curses.h>

int main()
{
    open_screen();
    begin_screen();
    pattern_screen();

    game_2048();
    close_screen();
}
