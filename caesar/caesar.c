#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool aDigit(string s);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    if(argc != 2 || !aDigit(argv[1]))
    {
        printf("Usage: ./cesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);

    string plaintxt = get_string("plaintext: ");

    printf("ciphertext: ");

    for (int i = 0, n = strlen(plaintxt); i < n; i++)
    {
        printf("%c", rotate(plaintxt[i], key));
    }

    printf("\n");
    return 0;
}

bool aDigit(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

char rotate(char c, int key)
{
    if (isupper(c))
    {
        return((c - 'A' + key) % 26) + 'A';
    }
    else if (islower(c))
    {
        return ((c - 'a' + key) % 26) + 'a';
    }
    else
    {
        return c;
    }
}
