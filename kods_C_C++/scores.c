#include <stdio.h>
#include <cs50.h>

float get_avr(int count, int array[]);

int main(void)
{
    float sum;
    int n = get_int("How many scores do you have? \n");
    int scores[n];

    for (int i = 0; i < n; i++)
    {
        scores[i] = get_int("Score [%i]?\n", i);
    }

    for (int j = 0; j < n; j++)
    {
        sum = sum + scores[j];
    }

    float avr = sum / n;
    printf("Average result: %f  \n", avr);
}


float get_avr(int count, int array[])
{
    int total = 0;
    for (int i = 0; i < count; i++)
    {
        total += array[i];
    }
    return total / (float)count;
}