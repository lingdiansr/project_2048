#include <stdio.h>
#define ROW 4
#define COL 4
int box[ROW][COL] = {
    4, 2, 0, 0,
    8, 4, 2, 0,
    4, 16, 4, 0,
    512, 32, 16, 4};
void up_combine()
{
}
void left_combine()
{
}
void right_combine()
{
}
void down_combine() // 向下合并
{
    int i, j, k;
    for (i = 0; i < COL; i++)
    {
        for (j = ROW - 2; j >= 0; j--)
        {
            for (k = j + 1; k <= ROW - 1; k++)
            {
                // 寻找合并位置
                // 1.下方经过或不经过空位有相同数字，合并到该位置
                // 2.下方有不同数字时，合并到上一个位置
                // 3.下方全为空，移动到该位置
                // if中为1. 2. 这两种情况需要提前跳出循环得到位置
                if (box[k][i] == box[j][i] || (box[k][i] != 0 && box[k][i] != box[j][i]))
                {
                    break;
                }
            }
            if (box[k][i] == box[j][i]) // 相同合并
            {
                box[k][i] *= 2;
                box[j][i] = 0;
            }
            else if (box[k][i] != 0 && box[k][i] != box[j][i]) // 不同移动到上一个位置
            {
                if (k > j + 1)
                { // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[k - 1][i] = box[j][i];
                    box[j][i] = 0;
                }
            }
            else // 移动到空位置
            {
                box[k][i] = box[j][i];
                box[j][i] = 0;
            }
        }
    }
}
void print_box() // 输出到屏幕上
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d\t", box[i][j]);
        }
        printf("\n");
    }
}
int main()
{
    print_box();
    printf("\n");
    down_combine();
    print_box();
    return 0;
}