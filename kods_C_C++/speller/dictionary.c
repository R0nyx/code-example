// Implements a dictionary's functionality
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// word count;
int word_count;

// SIZE value, how many words in

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_value = hash(word);
    node *ptr = table[hash_value];
    // IF word in dic return TRUE

    while (ptr != NULL)
    {
        if (strcasecmp(ptr->word, word) == 0) // check if word == word of the node
        {
            return true; // if true return
        }
        ptr = ptr->next; // if word != word , continue to next node;
    }
    // Else return false;
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    int first_char = toupper(word[0]) - 'A';
    if (first_char > 25)
    {
        return 26;
    }
    return first_char;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // 1. Open dictionary file;
    FILE *file_ptr = fopen(dictionary, "r");
    if (file_ptr == NULL)
    {
        return false;
    }
    //  2. Read strings from file on at a time
    char read_word[LENGTH + 1];
    int hash_value;
    int i = 0;

    // node *table[N];

    // read all the words from the file
    while (fscanf(file_ptr, "%s", read_word) != EOF)
    {
        //  3. Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(n);
            return false;
        }

        strcpy(n->word, read_word); // copy word to new node
        // printf("%s\n", n->word);

        //  4. Hash word to obtain a hash value
        hash_value = hash(read_word);

        //  5. insert node into hash table at that location
        n->next = table[hash_value];
        table[hash_value] = n;
        // count the words;
        word_count++;
    }

    // printf("Word Count : %i \n", word_count);
    fclose(file_ptr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *next_tmp_ptr = ptr->next;
            free(ptr);
            ptr = next_tmp_ptr;
        }
    }
    return 1;
}
