#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BYTE_INDEX(i) ((i) >> 3)
#define BIT_MASK(i) (1 << ((i) % 8))

bool bitarray_check(uint8_t bitarray[], int index)
{
    uint8_t byte = bitarray[BYTE_INDEX(index)];
    return byte & BIT_MASK(index);
}

void bitarray_set(uint8_t bitarray[], int index)
{
    bitarray[BYTE_INDEX(index)] |= BIT_MASK(index);
}

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

    // We need 2020 bits (252.5 bytes) to hold every possibility.
    uint8_t bitarray[253] = {0};

    while (fscanf(fp, "%d\n", &num) != EOF)
    {
        if (num >= 2020 || num < 1)
        {
            // Sanity.
            continue;
        }
        int target = 2020 - num;
        if (bitarray_check(bitarray, target))
        {
            printf("Found! %d x %d = %d\n", num, target, num * target);
            break;
        }
        else
        {
            bitarray_set(bitarray, num);
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}