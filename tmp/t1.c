#include <stdio.h>
char score_history[11][40];

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
        // 去掉换行符
        strtok(line, "\n");

        // 存储到字符串数组中
        strcpy(score_history[line_count], line);

        // 更新行数计数器
        line_count++;
    }

    fclose(fp); // 关闭文件
}
void write_score(unsigned long long socre) // 将得分记录写进本地文件
{
    FILE *fp;
    time_t now;
    time(&now);
    fp = fopen("history.txt", "w+");                        // 打开文件，如果不存在则创建一个新文件
    fprintf(fp, "时间：%s 分数：%llu", ctime(&now), socre); // 将分数写入文件
    fclose(fp);                                             // 关闭文件
}
int main()
{
    get_score_history();
    // write_score(10);
    return 0;
}