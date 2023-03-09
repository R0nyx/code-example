#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height:  \n");
    } while (height < 1 || height > 8);



    for (int i = 0; i < height; i++)
    {
        for (int spaces = height - 1; spaces > i; spaces--)
        {
            printf(" ");
        }

        for (int hash_l = -1; hash_l < i; hash_l++)
        {
            printf("#");
        }

        // Printing gap:
        printf("  ");

        for (int hash_r = -1; hash_r < i; hash_r++)
        {
            printf("#");
        }
        printf("\n");
    }
}
