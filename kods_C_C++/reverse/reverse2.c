#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    // string format = ".wav";
    char format[4] = ".wav";
    // printf("Input : %s\nOutput : %s\n", argv[1], argv[2]);

    if (argc < 2)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }
    else if (strstr(argv[1], format) != 0)
    {
        printf("Input is not a WAV file.\n");
    }
    // DECELARE INPUT AND OUTPUT FILES
    char *input_file = argv[1];
    char *output_file = argv[2];

    // Open input file for reading
    // TODO #2
    // Open input file (fopen opens, taken from <stdio.h>);
    // fopen opens file and returns file pointer;
    // r stands for read-only mode if you dont want to overwrite use "a";
    // Need to check if the file * returned is not NULL
    FILE *input_file_ptr = fopen(input_file, "r");
    if (input_file_ptr == NULL)
    {
        printf("Could not open %s.\n", input_file);
        return 1;
    }

    // Read header into an array
    // TODO #3
    // Read infile's HEADER
    WAVHEADER in_HEADER;
    //
    fread(&in_HEADER, sizeof(WAVHEADER), 1, input_file_ptr);

    // printf("ftell: %ld", ftell(input_file_ptr));

    // TODO #4 Use check_format to ensure WAV format
    if (check_format(in_HEADER) != 0)
    {
        printf("The file format DOES NOT match\n");
        return 2;
    }

    // TODO #5
    // Open output file for writing

    FILE *output_file_ptr = fopen(output_file, "w");
    if (output_file_ptr == NULL)
    {
        fclose(input_file_ptr);
        printf("Could not create %s.\n", output_file);
        return 5;
    }

    //

    // Write header to file
    // TODO #6
    fwrite(&in_HEADER, sizeof(WAVHEADER), 1, output_file_ptr);

    // THIS SIMPLY COMPIES THE FILE, Byte by byte
    // char buffer[block_size];
    /*while (fread(&buffer, sizeof(char), 1, input_file_ptr))
    {
        fwrite(&buffer, sizeof(char), 1, output_file_ptr);
    }
    */
    int block_size = get_block_size(in_HEADER);
    char buffer[block_size];
    fseek(input_file_ptr, block_size, SEEK_END);

    while (ftell(input_file_ptr) - block_size > sizeof(in_HEADER))
    {
        fseek(input_file_ptr, -2 * block_size, SEEK_CUR);
        fread(&buffer, 4, 1, input_file_ptr);
        fwrite(&buffer, 4, 1, output_file_ptr);

    }

    fclose(input_file_ptr);
    fclose(output_file_ptr);
    return 0;
}
// end main;

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'W' && header.format[3] == 'E')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int get_block_size(WAVHEADER header)
{
    // we calculate the size of each block with the following calculation:
    // number of channels multiplied by bytes per sample
    // TODO #7 Get block size number
    // printf("block size %i", (header.numChannels * (header.bitsPerSample / 8)));
    int block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}