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

void day_one_solution(FILE *fp)
{
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
}

void day_one_b_solution(FILE *fp)
{
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
}