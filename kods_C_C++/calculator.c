#include <cs50.h>
#include <stdio.h>

int main(void){
    float x = get_long("Enter first value\n");
    float y = get_long("Enter second value\n");

    printf("sum = %.15f", x / y);
}