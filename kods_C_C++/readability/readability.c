#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// decelarations
int count_characters(string text);
int count_words(string text);
int count_sentance(string text);
void get_grade(double ch, double words, double sent);
int length;

int main(void)
{
    string text = get_string("TEXT: \n");

    length = strlen(text);

    int characters = count_characters(text);
    int words = count_words(text);
    int sentances = count_sentance(text);
    get_grade(characters, words, sentances);

    // printf("%i letters\n", characters);
    // printf("%i words\n", words);
    // printf("%i sentances\n", sentances);
}

int count_characters(string text)
{
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        // lower case:
        if ((text[i] >= 97 && text[i] <= 122) || (text[i] >= 65 && text[i] <= 90))
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int words = 0;
    if (text[0] != ' ')
    {
        words++;
    }

    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ') // space can also be assumed as 32 ASCII;
        {
            words++;
        }
    }
    return words;
}

int count_sentance(string text)
{
    int sentances = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentances++;
        }
    }
    return sentances;
}

void get_grade(double ch, double words, double sent)
{
    double grade = 0;
    // to get L:
    double L = (ch / words) * 100;
    // to get S:
    double S = (sent / words) * 100;

    // Calculate Coleman-liau Index
    grade = 0.0588 * L - 0.296 * S - 15.8;

    grade = round(grade);

    if (grade >= 16)
    {
        printf("Grade 16+\n");
        return;
    }

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int)grade);
    }
}