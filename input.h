
#ifndef _INPUT_H_
#define _INPUT_H_



#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define QUIT -1
#define RESTART 5


#define NOR_MODE 101
#define LIT_MODE 102
#define EAS_MODE 103

int close_input();
int get_user_input();
int begain_input();
void pattern_input();
void set_size();
int ra();
#endif