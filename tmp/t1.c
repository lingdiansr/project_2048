#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    int rand;                 // 存储2位整数
    unsigned long long score; // 存储8位整数
    char time[25];
} score_mark;

score_mark score_history[11];

void get_score_history()
{
    char filename[] = "history.txt";
    FILE *fp;
    char line[40];

    fp = fopen(filename, "r"); // 打开文件
    if (fp == NULL)
    {
        // 文件没有成功打开时，新建一个文件
        fp = fopen(filename, "w+");
        fclose(fp);
        return;
    }

    // 逐行读取文件内容
    int line_count = 0;
    while (fgets(line, 40, fp) != NULL)
    {
        strtok(line, "\n"); // 去掉换行符

        line_count++; // 更新行数计数器
    }

    fclose(fp); // 关闭文件
    for (int i = 0; i < line_count; i++)
    {
        printf("%s", score_history[i]);
    }
}
void write_score(unsigned long long s) // 将得分记录写进本地文件
{
    FILE *fp;
    time_t now;
    unsigned long long score[11], temp;
    score[0] = s;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10 - i; j++)
        {
            if (score[j] < score[j + 1])
            {
                temp = score[j];
                score[j] = score[j + 1];
                score[j + 1] = temp;
            }
        }
    }

    fp = fopen("history.txt", "w+"); // 打开文件，如果不存在则创建一个新文件
    time(&now);
    fprintf(fp, "%2d. score:%8llu time:%s", score, strtok(ctime(&now), "\n")); // 将分数写入文件，并在后面加上一个空格
    fclose(fp);                                                                // 关闭文件
}
int main()
{
    get_score_history();
    // write_score(10);
    return 0;
}