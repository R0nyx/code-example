#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    string name;
    string number;
}person;

int main(void)
{

    person people[2];
    people[0].name = "carter";
    people[0].number = "1111111";

    people[1].name = "david";
    people[1].number = "222222";

    string s = get_string("Input name: \n");

    for (int i = 0; i < 2; i++)
    {
        if (strcmp(people[i].name, s) == 0)
        {
            printf("found %s\n", people[i].number);
            return 0;
        }
    }
    printf("Not found \n");
    return 1;
}