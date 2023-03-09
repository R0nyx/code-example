#include <cs50.h>
#include <stdio.h>
#include <string.h>


int main(void){
    //int numbers[] = {20, 500, 10, 5, 100, 1, 50};
    string names[] = {"carter","david"};
    string numbers[] = {"213123","333333"};


    //int n = get_int("Input number: \n");
    string s = get_string("Input name: \n");

    //Segmantaiton fault means too many searches of the array
    for (int i = 0; i < 2; i++){
        if(strcmp(names[i], s) == 0){
            printf("found %s\n", numbers[i]);
            return 0;
        }
    }
    printf("Not found \n");
    return 1;
}