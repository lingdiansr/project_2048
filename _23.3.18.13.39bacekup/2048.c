#include <stdlib.h>
#include <time.h>
const int ROW = 4;
const int COL = 4;
int box[ROW][COL] = {0};
struct empty_pos
{
    int x;
    int y;
} empty_sqe[ROW * COL]; // 记录空位置坐标
void empty_init()       // 位置初始化
{
    for (int i = 0; i < ROW * COL; i++)
    {
        empty_sqe[i].x = 0;
        empty_sqe[i].y = 0;
    }
}
void bor() // 随机化box
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            srand((unsigned int)time(NULL) + rand());
            box[i][j] = rand() % 9 ? 2 : 4;
        }
    }
}
int get_empty() // 获取空位置数量并把位置记录在sqe中
{
    int n = 0;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (box[i][j] == 0)
            {
                n++;
                empty_sqe[n - 1].x = i;
                empty_sqe[n - 1].y = j;
            }
        }
    }
    return n;
}
void print_scr() // 输出到屏幕上
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }
}
int random_num() // 随机生成2或4
{
    srand(time(NULL));
    return (rand() % 2 + 1) * 2;
}
void fill_rand_num() // 填入随机2/4
{
    srand(time(NULL));
    int n = get_empty();
    int pos = rand() % n;
    for (int i = 0; i < n; i++)
    {
        if (i + 1 == pos)
        {
            box[empty_sqe[i].x][empty_sqe[i].y] = random_num();
        }
    }
}
void down_combine() // 向下合并
{

    for (int i = ROW - 1; i > 0; i--)
    {
        for (int j = 0; j < COL; j++)
        {
            if (box[i][j] == box[i - 1][j]) // 如果上面的与下面的相同，合并，上面置零
            {
                box[i][j] *= 2;
                box[i - 1][j] = 0;
            }
            else if (box[i][j] == 0 && box[i - 1][j] != 0) // 如果下面为空，那么上面的下移
            {
                box[i][j] = box[i - 1][j];
                box[i - 1][j] = 0;
            }
        }
    }
}
