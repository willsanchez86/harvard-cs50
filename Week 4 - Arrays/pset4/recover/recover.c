#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    // Check Usage
    if (argc != 2)
    {
        printf("Usage: recover infile\n");
        return 1;
    }

    // Open File and check if readable
    char *infile = argv[1];
    FILE *file = fopen(infile, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Initilize variables
    int counter = 0;
    unsigned char block[512];
    FILE *img = NULL;

    while (fread(&block, sizeof(block), 1, file) != 0)
    {
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {
            // // Close previous file if not the first
            if (counter != 0)
            {
                fclose(img);
            }

            char filename[8];
            // Create the new jpg file
            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");

            // Update image counter
            counter++;
        }

        // If new jpeg has been initialized, write the blocks into the file
        if (counter != 0)
        {
            fwrite(&block, sizeof(block), 1, img);
        }
    }
    fclose(img);
    fclose(file);

    return 0;
}