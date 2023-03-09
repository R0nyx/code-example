#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"

char *table[LENGTH + 1];
int count_words = 0;

int main(int argc, char *argv[])
{
    char *input_file = argv[1];
    FILE *file_ptr = fopen(input_file, "r");
    if (file_ptr == NULL)
    {
        printf("Could not open %s.\n", input_file);
        return false;
    }
    //  2. Read strings from file on at a time
    char read_word[100];
    int hash_value;
    int i = 0;

    // read all the words from the file
    while (fscanf(file_ptr, "%s", read_word) != EOF)
    {
        table[i] = malloc(sizeof(char) * (strlen(read_word) + 1));
        strcpy(table[i], read_word);
        printf("%s\n", read_word);
        i++;
        count_words++;
    }

    for(int j=0; j<i; j++){
        printf("%s\n",table[j]);
    }
    printf("word count: %i\n", count_words);

    fclose(file_ptr);
}