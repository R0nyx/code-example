#include <stdio.h>
#include <cs50.h>
#include <string.h>
//You can use string.h to get length of the string.

int main(void){
    string name = get_string("Whats your name? \n");
    int n = strlen(name);

    printf("%i", n);


    //while (name[n] != '\0'){
    //    n++;
    //}
    //printf("Length of string: %i\n", n);
}