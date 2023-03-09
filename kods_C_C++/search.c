#include <cs50.h>
#include <stdio.h>
#include <string.h>


int main(void){
    //int numbers[] = {20, 500, 10, 5, 100, 1, 50};
    string strings[] = {"battleship",  "boot", "cannon", "iron", "thimble","tophat" };

    //int n = get_int("Input number: \n");
    string s = get_string("Input number: \n");

    //Segmantaiton fault means too many searches of the array
    for (int i = 0; i < 6; i++){
        if(strcmp(strings[i], s) == 0){
            printf("found\n");
            return 0;
        }
    }
    printf("Not found \n");
    return 1;
}