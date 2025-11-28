#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    float L = (float) letters * 100.0f / (float) words; // Problem i found need to make sure
    float S = (float) sentences * 100.0f / (float) words; // this is in float NOT INT it roud up to 8 if not FLOAT

    float index = roundf(0.0588 * L - 0.296 * S - 15.8);

    /*
    Debugging
    printf("Number of LETTERS: %i\n", letters);
    printf("Number of WORDS: %i\n", words);
    printf("Number of SENTENCES: %i\n", sentences);
    */

    // Print the grade level

    if (index < 1)
        printf("Before Grade 1\n");
    else if (index >= 16)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", (int) index);
}

int count_letters(string text)
{
    int letters = 0;
    // Return the number of letters in text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i])) // check whether a character is alphabetical
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 1; // Need to start at one word, doesnt count the first one

    // Return the number of words in text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i])) // check whether a character is blank
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    // Return the number of sentences in text
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' ||
            text[i] == '?') // check whether a character is punctuation
        {                   // We cant use ispunct because it count ',' and we only want '.' '!' '?'
            sentences++;
        }
    }
    return sentences;
}
