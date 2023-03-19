#include <stdio.h>
#include <curses.h>

// 绘制窗格
void draw_grid(void)
{
    int m, n, x, y;
    clear();//将整个屏幕清除
    //绘制横线 m行 n列
    for(m = 0; m < 9; m += 8){
        for(n = 0; n < 21; n++){
            move(m,n);//将光标移动至 m,n 的位置.
            addch('-');//在当前光标位置输入单个字符，并将光标右移一位。
            refresh();//刷新屏幕,使绘制即时生效
        }
    }
    //绘制竖线
    for (n = 0; n < 22; n += 20){
        for(m = 1; m < 8; m++){
            move(m,n);
            addch('|');
            refresh();
        }
    }
}

int main()
{
	int x0=4,y0=4;


    initscr();
    noecho(); 

    WINDOW *win_begin=newwin(100,100,y0,x0);
	//draw_grid();
	curs_set(0);
	wmove(win_begin,0,5);
	waddstr(win_begin,"WELCOME TO 2048");
	wmove(win_begin,4,4);
	waddstr(win_begin,"1.Normal mode: enter p");
	wmove(win_begin,8,4);
	waddstr(win_begin,"2.Cheat mode: enter c");
	wmove(win_begin,12,4);
	waddstr(win_begin,"3.Control centre: enter i");

	wrefresh(win_begin);

    keypad(stdscr,TRUE);
    while(1);
    

	return 0;
}
