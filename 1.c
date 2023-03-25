#include<stdio.h>
#include<time.h>
#include<stdlib.h>
void bor(int a[]) // 随机化matrix
{
	int num,i,j;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 0; j < 4; j++)
        {
            srand((unsigned int)time(NULL) + rand());
            num=rand() % 9 ? 2 : 4;
        }
    }
}
int main()
{
	int i,a[10]={0};
	 bor(a);
	 for(i=0;i<10;i++)
	 printf("%d",a[i]);
	 return 0;
}
