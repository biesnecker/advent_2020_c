#include <stdio.h>
#include <string.h>
#include <stdint.h>

int popcnt(uint32_t v)
{
    // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    v = v - ((v >> 1) & 0x55555555);
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

void day_six_solution(FILE *fp)
{
    char buffer[50] = {0};
    int total_sum = 0;

    uint32_t seen = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        int len = strlen(buffer);
        if (len < 2)
        {
            // newline
            total_sum += popcnt(seen);
            seen = 0;
            continue;
        }

        for (int i = 0; i < len - 1; ++i)
        {
            int v = buffer[i] - 'a';
            seen |= 1L << v;
        }
    }

    // Get the last group.
    total_sum += popcnt(seen);

    printf("Solution: %d\n", total_sum);
}

void day_six_b_solution(FILE *fp)
{
    char buffer[50] = {0};
    int lines = 0;
    int total_sum = 0;

    uint8_t counts[26] = {0};

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        int len = strlen(buffer);
        if (len < 2)
        {
            // newline
            if (lines > 0)
            {
                for (int i = 0; i < 26; ++i)
                {
                    total_sum += counts[i] == lines;
                }
                lines = 0;
                memset(counts, 0, sizeof(counts));
            }
            continue;
        }

        for (int i = 0; i < len - 1; ++i)
        {
            int idx = buffer[i] - 'a';
            counts[idx] += 1;
        }
        ++lines;
    }

    // Get the last group.
    if (lines > 0)
    {
        for (int i = 0; i < 26; ++i)
        {
            total_sum += counts[i] == lines;
        }
        lines = 0;
    }

    printf("Solution: %d\n", total_sum);
}