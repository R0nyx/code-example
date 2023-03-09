#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int lamas;
    int end_lamas;
    int years = 0;

    do
    {
        lamas = get_int("Start size: ");
    } while (lamas <= 9);

    // TODO: Prompt for end size

    do
    {
        end_lamas = get_int("End size: ");
    } while (end_lamas < lamas);

    while (lamas < end_lamas)
    {
        int gain = lamas / 3;
        int loss = lamas / 4;
        lamas = lamas + (gain - loss);
        years++;
    }

    // do
    //{
    //     years++;
    //     int gain = lamas / 3;
    //     int loss = lamas / 4;
    //     lamas = lamas + (gain - loss);
    // } while (end_lamas >= lamas);

    // TODO: Print number of years
    printf("Years: %i \n", years);
}
