#include "2048.h"
#include "input.h"
#include "screen.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>
void random_num() // 随机生成2和4
{
    time_t t;                  // 获取系统时间作为种子参数
    srand((unsigned)time(&t)); // 随机种子
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", (rand() % 2 + 1) * 2);
    }
}
int main()
{
    random_num();
    open_screen();
    WINDOW *win = newwin(16, 32, 5, 5);

    print_screen();
}
