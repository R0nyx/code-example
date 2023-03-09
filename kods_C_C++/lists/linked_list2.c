#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int number;
    struct node *next;
} node;

int main(int argc, char *argv[])
{

    // Variable called list, init to NULL;
    node *list = NULL;

    for (int i = 1; i < argc; i++) // start at 1, argc = 0 == name of program.
    {
        int number = atoi(argv[i]); // atoi conversts ascii to int

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 1;
        }

        n->number = number; // nodes number == arg number
        n->next = NULL;     // clear garbage; END OF LIST

        n->next = list; // points to the next node
        list = n;       // change last number;
    }

    for (node *ptr = list; ptr != NULL; ptr = ptr->next)
    {
        printf("%i\n", ptr->number);
        ptr = ptr->next;
    }

    //node *ptr = list;

    while (ptr != NULL)
    {
        node *next_tmp_ptr = ptr->next;
        free(ptr);
        ptr = next_tmp_ptr;
    }

    return 0;
}