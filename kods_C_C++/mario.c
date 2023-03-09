#include <stdio.h>
#include <cs50.h>
int main(void)
{

    for (int i = 0; i < 4; i++)
    {
        printf("?\n");
    }
    printf("\n");

    const int square = 5;
    for (int i = 0; i < square; i++)
    {
        for (int j = 0; j < square; j++)
        {
            printf("X ");
        }
        printf("\n");
    }

    int num;

    do
    {
        num = get_int("Size? \n");
    } while (num < 1);

    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            printf("X ");
        }
        printf("\n");
    }
}