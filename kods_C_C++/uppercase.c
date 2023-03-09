#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// Upper case conversion can be done usng #include <ctype.h>

int main(void)
{
    string s = get_string("Before: ");
    printf("After:  ");
    string s2 = s;
    printf("%s", s2);

    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            printf("%c", s[i] -= 32);
        }
        else
        {
            printf("%c", s[i]);
        }
    }

    printf("\n");
    printf("other implementation:");

    for (int j = 0; j < strlen(s2); j++)
    {
        printf("%c", toupper(s2[j]));
    }
    printf("\n");
}