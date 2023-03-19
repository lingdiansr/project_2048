#ifndef _2048_H_
#define _2048_H_
#define DIFFICUITY 0
int get_empty();      // 位置初始化
void print_box();     // 输出到屏幕上
int random_num();     // 随机生成2或4
void fill_rand_num(); // 填入随机2/4
void up_combine();    // 向下合并
void left_combine();  // 向左合并
void right_combine(); // 向右合并
void down_combine();  // 向下合并
void game();

#endif