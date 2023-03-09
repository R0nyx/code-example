#include <cs50.h>
#include <stdio.h>
#include <string.h>

void draw(int n);

int main(void)
{

    int height = get_int("Height: \n");
    draw(height);
    printf("\n");
}

void draw(int n)
{
    if (n <= 0)
    {
        return;
    }
    draw(n - 1);

    for (int i = 0; i < n; i++)
    {
        printf("X");
    }
    printf("\n");
}