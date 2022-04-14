// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table (Originally 143091)
const unsigned int N = 143091;

// Hash table
node *table[N];

// Global word_count variable to be updated while loading
unsigned int Word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor -> next;
    }
    return false;
}

// CITATION:
// djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
// this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
// another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
// (why it works better than many other constants, prime or not) has never been adequately explained.
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // return toupper(word[0]) - 'A';
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open Dictionary File

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // Read strings from from file one at a time
    char dict_string[LENGTH + 1];
    while (fscanf(file, "%s", dict_string) != EOF)
    {
        // Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, dict_string);
        n -> next = NULL;
        // Hash word to obtain a hash value
        int hash_index = hash(dict_string);

        if (table[hash_index] == NULL)
        {
            table[hash_index] = n;
        }
        else
        {
            n -> next = table[hash_index];
            table[hash_index] = n;
        }
        Word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return Word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }
    }
    return true;
}
