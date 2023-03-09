#include "helpers.h"
#include <math.h>
#include <stdio.h>

#define RED 1
#define GREEN 2
#define BLUE 3

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avrg = 0;
    // Take average or red green and blue use that for average color
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avrg = round(((image[i][j].rgbtBlue) + (image[i][j].rgbtRed) + (image[i][j].rgbtGreen)) / 3.0);
            image[i][j].rgbtGreen = avrg;
            image[i][j].rgbtBlue = avrg;
            image[i][j].rgbtRed = avrg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE old_row[height][width];
    for (int i = 0; i < height; i++) // loop over every column
    {
        for (int j = 0; j < width; j++) // loop over every row
        {
            old_row[i][j] = image[i][j];
        }
    }
    // store temp values
    RGBTRIPLE col;

    for (int i = 0; i < height; i++) // every column
    {
        for (int j = 0; j < width; j++) // every row
        {
            col = old_row[i][j];
            image[i][width - 1 - j] = col;
        }
    }
    /*
    for (int i = 0; i < height; i++) // Loop over every line
    {
        for (int j = 0; j < width / 2; j++) // Loop over every pixel in line
        {
            RGBTRIPLE tmp = image[i][j];           // initiaL tmp color of the pixel
            image[i][j] = image[i][width - 1 - j]; // the initial pixel [i][j] = the furthest pixel[Width -1 -j]
            image[i][width - 1 - j] = tmp;         // make furthest pixel equal to initial temp
        }
    }
    */
    return;
}

RGBTRIPLE get_blur(int row, int column, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel_colour;
    float block_count = 0;
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    for (int box_row = row - 1; box_row <= (row + 1); box_row++)
    {
        for (int box_column = column - 1; box_column <= (column + 1); box_column++)
        {
            if (box_row < 0 || box_row >= height || box_column < 0 || box_column >= width)
            {
                continue;
            }
            r_sum += image[box_row][box_column].rgbtRed;
            g_sum += image[box_row][box_column].rgbtGreen;
            b_sum += image[box_row][box_column].rgbtBlue;
            block_count++;
        }
    }
    pixel_colour.rgbtRed = round(r_sum / block_count);
    pixel_colour.rgbtGreen = round(g_sum / block_count);
    pixel_colour.rgbtBlue = round(b_sum / block_count);

    return pixel_colour;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int row = 0; row < height; row++) // loop over every column
    {
        for (int column = 0; column < width; column++) // loop over every pixel
        {
            copy[row][column] = image[row][column];
        }
    }

    for (int row = 0; row < height; row++) // loop over every column
    {
        for (int column = 0; column < width; column++) // loop over every pixel
        {
            // printf("row : %i \ncolumn : %i\n" ,row, column);
            image[row][column] = get_blur(row, column, height, width, copy);
        }
    }

    return;
}

int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

int get_gx(int row, int column, int height, int width, RGBTRIPLE image[height][width], int col)
{
    int count_g = 0;
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    for (int box_row = row - 1; box_row <= (row + 1); box_row++)
    {
        for (int box_column = column - 1; box_column <= (column + 1); box_column++)
        {
            if (box_row < 0 || box_row >= height || box_column < 0 || box_column >= width)
            {
                count_g++;
                continue;
            }
            b_sum = b_sum + Gx[count_g] * (image[box_row][box_column].rgbtBlue);
            r_sum = r_sum + Gx[count_g] * (image[box_row][box_column].rgbtRed);
            g_sum = g_sum + Gx[count_g] * (image[box_row][box_column].rgbtGreen);

            count_g++;
        }
    }

    if (col == RED)
    {
        return r_sum;
    }
    else if (col == GREEN)
    {
        return g_sum;
    }
    else if (col == BLUE)
    {
        return b_sum;
    }
    return 5;
}

int get_gy(int row, int column, int height, int width, RGBTRIPLE image[height][width], int col)
{
    int count_g = 0;
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    for (int box_row = row - 1; box_row <= (row + 1); box_row++)
    {
        for (int box_column = column - 1; box_column <= (column + 1); box_column++)
        {
            if (box_row < 0 || box_row >= height || box_column < 0 || box_column >= width)
            {
                count_g++;
                continue;
            }
            b_sum = b_sum + Gy[count_g] * (image[box_row][box_column].rgbtBlue);
            r_sum = r_sum + Gy[count_g] * (image[box_row][box_column].rgbtRed);
            g_sum = g_sum + Gy[count_g] * (image[box_row][box_column].rgbtGreen);

            count_g++; // block_count++;
        }
    }

    if (col == RED)
    {
        // printf("r_sum (gY) = %i\n", r_sum);
        return r_sum;
    }
    if (col == GREEN)
    {
        // printf("g_sum = %i\n", g_sum);
        return g_sum;
    }
    if (col == BLUE)
    {
        //printf("b_sum = %i\n", b_sum);
        return b_sum;
    }
    return 4;
}
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx_r, gx_g, gx_b;
    int gy_r, gy_g, gy_b;

    // Make copy of the original image
    RGBTRIPLE copy[height][width];
    for (int row = 0; row < height; row++) // loop over every column
    {
        for (int column = 0; column < width; column++) // loop over every pixel
        {
            copy[row][column] = image[row][column];
        }
    }

    for (int row = 0; row < height; row++) // loop over every column
    {
        for (int column = 0; column < width; column++) // loop over every pixel
        {
            gx_r = get_gx(row, column, height, width, copy, RED);
            gx_g = get_gx(row, column, height, width, copy, GREEN);
            gx_b = get_gx(row, column, height, width, copy, BLUE);
            gy_r = get_gy(row, column, height, width, copy, RED);
            gy_g = get_gy(row, column, height, width, copy, GREEN);
            gy_b = get_gy(row, column, height, width, copy, BLUE);

            int r = round(sqrt((gx_r * gx_r) + (gy_r * gy_r)));
            if(r>255){r=255;}
            int g = round(sqrt((gx_g * gx_g) + (gy_g * gy_g)));
            if(g>255){g=255;}
            int b = round(sqrt((gx_b * gx_b) + (gy_b * gy_b)));
            if(b>255){b=255;}
            image[row][column].rgbtRed = r;
            image[row][column].rgbtGreen = g;
            image[row][column].rgbtBlue = b;
        }
    }
    return;
}
