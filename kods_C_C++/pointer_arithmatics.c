#include <stdio.h>
//#include <cs50.h>

//pointer really holds address of a value;
int main(void){

    char *s = "HI!";
    printf("%s\n", s);
    printf("%s\n", s+1);
    printf("%s\n", s+2);

}