#include <cs50.h>
#include <stdio.h>

int main(void){
    string answer = get_string("whats your name?\n");
    string answer2 = get_string("whats your last name?\n");
    printf("hello, %s %s\n", answer, answer2);
}