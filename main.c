#include "2048.h"
#include "input.h"
#include "screen.h"

#include <stdio.h>
#include <curses.h>

int main()
{
    open_screen();
    //begin_screen();
        // draw_grid();
        // while (1)
        // {
        //     /* code */
        // }
    
    game_2048();
    close_screen();
}
