#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check for command line args
    if (argc != 2)
    {
        printf("Usage: ./read infile\n");
        return 1;
    }

    int plate_lenght= 7;
    // Create buffer to read into
    char buffer[plate_lenght];
    //READS array of characters - representing one plate

    // Create array to store plate numbers
    char *plates[8]; // strings of plate numbers

    FILE *infile = fopen(argv[1], "r");

    int idx = 0;

    while (fread(buffer, 1, plate_lenght, infile))
    {

        // Replace '\n' with '\0'
        buffer[6] = '\0';

        // Dynamically allocate memory for the string;
        plates[idx] = (char *)malloc(plate_lenght * sizeof(char));
        strcpy(plates[idx], buffer);
        idx++;
    }

    for (int i = 0; i < 8; i++)
    {
        printf("%s\n", plates[i]);
    }

    fclose(infile);
}
