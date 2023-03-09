#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word, string word2);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int result = compute_score(word1, word2);
    // int score2 = compute_score(word2);

    // TODO: Print the winner
    if (result == 1)
    {
        printf("Player 1 wins!\n");
    }
    else if (result == 2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word1, string word2)
{
    int sum1;
    int sum2;

    // make all letters lower case
    for (int i = 0; i < strlen(word1); i++)
    {
        word1[i] = tolower(word1[i]);
    }

    for (int j = 0; j < strlen(word2); j++)
    {
        word2[j] = tolower(word2[j]);
    }

    // WORD1 score:
    for (int i = 0; i < strlen(word1); i++)
    {
        if (word1[i] < 97 || word1[i] > 122)
        {
            sum1 = sum1 + 0;
        }
        else
        {
            sum1 += POINTS[word1[i] - 'a'];
        }
    }
    // WORD2 score:
    for (int i = 0; i < strlen(word2); i++)
    {
        if (word2[i] < 97 || word2[i] > 122)
        {
            sum2 = sum2 + 0;
        }
        else
        {
            sum2 += POINTS[word2[i] - 'a'];
        }
    }
    printf("score1: %i\nscore2: %i\n", sum1, sum2);

    // compare score
    if (sum1 > sum2)
    {
        return 1;
    }
    else if (sum2 > sum1)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}
