#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    int cent;
    do
    {
        cent = get_float("Change Owed: "); // We ask the user to introduce a number must be positive
        if (cent < 0)
        {
            printf("Introduce a positve number\n"); // If we introduce a non-valid numer message will apear
        }
    }
    while (cent < 0);

    int coins = 0;

    coins += cent / 25;
    cent %= 25;

    coins += cent / 10;
    cent %= 10;

    coins += cent / 5;
    cent %= 5;

    coins += cent;

    printf("%i\n", coins);
}
