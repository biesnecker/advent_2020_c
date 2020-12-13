#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/queue.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        return EXIT_FAILURE;
    }

    int num = 0;

    uint16_t values[2020] = {0};

    while (fscanf(fp, "%d\n", &num) != EOF)
    {
        if (num >= 2020 || num < 1)
        {
            // Sanity.
            continue;
        }
        // Loop through the existing numbers.
        for (int i = 0; i < 2020; ++i)
        {
            if (values[i] == 0)
            {
                continue;
            }
            else if (values[i] == i)
            {
                // This value has been seen before but isn't the sum of different numbers.
                int index = i + num;
                if (index < 2020)
                {
                    values[index] = i;
                }
            }
            else if (values[i] != i)
            {
                // This value is the sum of different numbers.
                if (values[i] + (i - values[i]) + num == 2020)
                {
                    int x = values[i];
                    int y = i - values[i];
                    printf("Found! %d x %d x %d = %d\n", num, x, y, x * y * num);
                    break;
                }
            }
        }
        if (values[num] == 0)
        {
            values[num] = num;
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}