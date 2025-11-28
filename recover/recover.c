#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Usage check
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open input file
    FILE *in = fopen(argv[1], "rb");
    if (in == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    BYTE buffer[BLOCK];
    FILE *out = NULL;
    int img_count = 0;
    char filename[8]; // "000.jpg" + '\0' = 8

    // Read the card raw data block by block
    while (fread(buffer, 1, BLOCK, in) == BLOCK)
    {
        // Check for JPEG header
        bool is_jpeg =
            buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0;

        if (is_jpeg)
        {
            // If already writing a JPEG, close it
            if (out != NULL)
            {
                fclose(out);
            }

            // Create a new JPEG file
            sprintf(filename, "%03i.jpg", img_count++);
            out = fopen(filename, "wb");
            if (out == NULL)
            {
                fclose(in);
                printf("Could not create output file.\n");
                return 1;
            }
        }

        // If we've started finding JPEGs, keep writing blocks
        if (out != NULL)
        {
            fwrite(buffer, 1, BLOCK, out);
        }
    }

    // Clean up
    if (out != NULL)
    {
        fclose(out);
    }
    fclose(in);
    return 0;
}
