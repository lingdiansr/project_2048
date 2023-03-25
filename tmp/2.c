#include <stdio.h>

#define OPTION_A 0
#define OPTION_B 1
#define size 5
int main(void)
{
    int option = 0;

    printf("Enter an option (0 for Option A, 1 for Option B): ");
    scanf("%d", &option);


    if (option == OPTION_A)
    {
        printf("%d\n", size);
#ifdef size 
#undef size
#define size 3
#endif
        printf("%d\n", size);
    }



    return 0;
}
