#include <stdio.h>
#include <stdbool.h>
#define ROW 4
#define COL 4
int box[ROW][COL] = {
    2, 2, 2, 4,
    4, 8, 16, 0,
    2, 0, 2, 0,
    256, 0, 0, 2};
bool up_combine() // 向上合并
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < COL; i++)
    {
        for (j = 1; j <= ROW - 1; j++) //*
        {
            for (k = j - 1; k > 0; k--) //*
            {
                // 寻找合并位置
                // 1.上方经过或不经过空位有相同数字，合并到该位置
                // 2.上方有不同数字时，合并到上一个位置
                // 3.上方全为空，移动到该位置
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
                flag = true;
            }
            else if (box[k][i] != 0 && box[k][i] != box[j][i]) // 不同移动到上一个位置
            {
                if (k < j - 1)                 //*
                {                              // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[k + 1][i] = box[j][i]; //*
                    box[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[k][i] = box[j][i];
                box[j][i] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
//  4  4  2  2 

//  8 4 2 8



bool down_combine() // 向下合并
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < COL; i++)
    {
        for (j = ROW - 2; j >= 0; j--)
        {
            for (k = j + 1; k < ROW - 1; k++)
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
                flag = true;
            }
            else if (box[k][i] != 0 && box[k][i] != box[j][i]) // 不同移动到上一个位置
            {
                if (k > j + 1)
                { // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[k - 1][i] = box[j][i];
                    box[j][i] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[k][i] = box[j][i];
                box[j][i] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool left_combine()
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < ROW; i++)
    {
        for (j = 1; j <= COL - 1; j++)
        {
            for (k = j - 1; k > 0; k--)
            {
                if (box[i][k] == box[i][j] || (box[i][k] != 0 && box[i][k] != box[i][j]))
                {
                    break;
                }
            }
            if (box[i][k] == box[i][j]) // 相同合并
            {
                box[i][k] *= 2;
                box[i][j] = 0;
                flag = true;
            }
            else if (box[i][k] != 0 && box[i][k] != box[i][j]) // 不同移动到上一个位置
            {
                if (k < j - 1)                 //*
                {                              // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[i][k + 1] = box[i][j]; //*
                    box[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[i][k] = box[i][j];
                box[i][j] = 0;
                flag = true;
            }
        }
    }
    return flag;
}
bool right_combine()
{
    int i, j, k;
    bool flag = false;
    for (i = 0; i < ROW; i++)
    {
        for (j = COL-2; j >=0; j--)
        {
            for (k = j + 1; k <COL-1; k++)
            {
                if (box[i][k] == box[i][j] || (box[i][k] != 0 && box[i][k] != box[i][j]))
                {
                    break;
                }
            }
            if (box[i][k] == box[i][j]) // 相同合并
            {
                box[i][k] *= 2;
                box[i][j] = 0;
                flag = true;
            }
            else if (box[i][k] != 0 && box[i][k] != box[i][j]) // 不同移动到上一个位置
            {
                if (k > j + 1)                 //*
                {                              // 相邻时不需要变化，此条件不可写入外层elif，否则会误判进else情况
                    box[i][k - 1] = box[i][j]; //*
                    box[i][j] = 0;
                    flag = true;
                }
            }
            else // 移动到空位置
            {
                box[i][k] = box[i][j];
                box[i][j] = 0;
                flag = true;
            }

        }
        
    }
    return flag;
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
    printf("\n");
}
int main()
{
    print_box();
    printf("\n");
    right_combine();
    print_box();
    return 0;
}