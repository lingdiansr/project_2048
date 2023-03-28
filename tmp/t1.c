#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // 添加时间头文件

// 结构体定义
typedef struct
{
    char year[5]; // 年份
    char mon[4];  // 月份
    char day[3];  // 天数
    char hour[3]; // 小时
    char min[3];  // 分钟
} time_info;      // 时间
typedef struct 
{
    int rand;                 // 排名
    unsigned long long score; // 分数
    time_info time;
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
    while (fgets(line, 40, fp) != NULL && line_count < 10) // 修改条件，只读取前10行
    {
        strtok(line, "\n"); // 去掉换行符

        // 将读取到的内容按照格式存储到score_history数组中
        sscanf(line, "%d. score:%llu time:%[^-]-%[^-]-%[^-]-%[^-]-%[^-]", &score_history[line_count].rand, &score_history[line_count].score, score_history[line_count].time.year, score_history[line_count].time.mon, score_history[line_count].time.day, score_history[line_count].time.hour, score_history[line_count].time.min);

        line_count++; // 更新行数计数器
    }
    fclose(fp); // 关闭文件
}
void write_score(unsigned long long s) // 将得分记录写进本地文件
{
    FILE *fp;
    time_t now;
    time(&now);
    score_mark temp;
    score_history[10].score = s;
    score_history[10].rand = 11;
    char *time_str = strtok(ctime(&now), "\n");
    for (int i = 0; i < strlen(time_str); i++)
    {
        if (time_str[i] == ' ')
        {
            time_str[i] = '-';
        }
    }
    strcpy(score_history[10].time, time_str);
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 10 - i; j++)
        {
            if (score_history[j].score < score_history[j + 1].score) // 修改比较条件
            {
                temp = score_history[j];
                score_history[j] = score_history[j + 1];
                score_history[j + 1] = temp;
            }
        }
    }

    fp = fopen("history.txt", "w+"); // 打开文件，如果不存在则创建一个新文件

    for (int i = 0; i < 10; i++)
    {
        printf("%d. score:%llu time:%s\n", i + 1, score_history[i].score, strtok(score_history[i].time, "\n"));
        fprintf(fp, "%d. score:%llu time:%-s\n", i + 1, score_history[i].score, strtok(score_history[i].time, "\n"));
    }
    fclose(fp);
}
int main()
{
    get_score_history();
    //for (int i = 0; i < 10; i++)
    printf("%s", score_history[0].time);


    return 0;
}