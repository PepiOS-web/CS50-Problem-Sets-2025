#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;
    do
    {
        h = get_int("Height: ");
        if (h < 1 || h > 8)
        {
            printf("Introduce a number between 1 and 8 \n");
        }
    }

    while (h < 1 || h > 8);

    for (int i = 1; i <= h; i++)
    {
        // This makes spaces wich decresee the rows
        for (int s = 0; s < h - i; s++)
        {
            printf(" ");
        }
        // blocks increse each row
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
