#include <stdio.h>
#include <cs50.h>

int main(int argc, string argv[]) //argc count of argument, string commands;
{
    //printf("Hello, %s\n", argv[1], argv[2]); //program name;
    if(argc == 2){
        printf("hello, %s \n", argv[1]);
    }
    else{
        printf("\nhello, world");
    }
}