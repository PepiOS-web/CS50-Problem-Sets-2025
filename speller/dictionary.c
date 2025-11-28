// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int words_in_dict = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int idx;
    node *cursor;
    char temp[LENGTH + 1];
    int i;
    int len;

    // 1️⃣ Convertir la palabra a minúsculas (para comparar sin distinguir mayúsculas)
    len = strlen(word);
    for (i = 0; i < len; i++)
    {
        temp[i] = tolower(word[i]);
    }
    temp[len] = '\0';

    // 2️⃣ Calcular el índice hash de la palabra
    idx = hash(temp) % N;

    // 3️⃣ Recorrer la lista enlazada en esa posición
    cursor = table[idx];
    while (cursor != NULL)
    {
        // strcmp devuelve 0 si son iguales
        if (strcmp(cursor->word, temp) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    // 4️⃣ Si no la encuentra, no está en el diccionario
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long h = 5381;
    int c;
    int i = 0;

    while (word[i] != '\0')
    {
        c = tolower((unsigned char) word[i]);
        h = ((h << 5) + h) + c;
        i++;
    }

    return (unsigned int) (h % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char buffer[LENGTH + 1]; // Temporary space to read the word

    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    if (file == NULL)
    {
        return false;
    }

    // Read each word in the file
        int scanned = fscanf(file,"%45s", buffer);

    while (scanned != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        //Copy each word to the node to the buffer

        strcpy(n->word, buffer);

        int index = hash(buffer) % N;

        n->next = table[index];
        table[index] = n;

        words_in_dict++;

        scanned = fscanf(file, "%45s", buffer);
    }

    // Close the dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words_in_dict;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor;

    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
        table[i] = NULL;
    }

    words_in_dict = 0;
    return true;
}
