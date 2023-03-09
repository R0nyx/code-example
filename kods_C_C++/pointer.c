#include <stdio.h>
//#include <cs50.h>

//pointer really holds address of a value;
int main(void){
    int n = 50;
    int *p = &n;
    //printf("%p - %i\n", &n, n);
    //printf("%p\n", p);
    //printf("%i\n", *p);
    //string s = "Hi!"; //string is essentially the same char *s = "HI!";
    //printf("%p\n", p);
    char *s = "HI!";
    printf("%s\n%p\n", s, s);
    printf("%p\n", &s[0]);
    printf("%p\n", &s[1]);
    printf("%p\n", &s[2]);
    printf("%p\n", &s[3]);

}