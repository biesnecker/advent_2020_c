#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "macros.h"

void day_three_solution(FILE *fp)
{
    char buffer[100] = {0};

    char *first_line = fgets(buffer, 100, fp);
    if (first_line == NULL)
    {
        FLOG("First line was empty?!\n");
        return;
    }

    int tree_count = 0;

    int pos = 3;

    // The line length is really one less than strlen returns, because of the
    // newline character.
    int len = strlen(first_line) - 1;
    while (fgets(buffer, 100, fp) != NULL)
    {
        // Overwrite the newline with a null.
        buffer[len] = '\0';

        int index = pos % len;
        tree_count += buffer[index] == '#';
        pos += 3;
    }

    printf("Trees encountered: %d\n", tree_count);
}

void day_three_b_solution(FILE *fp)
{
    char buffer[100] = {0};

    char *first_line = fgets(buffer, 100, fp);
    if (first_line == NULL)
    {
        FLOG("First line was empty?!\n");
        return;
    }
    // The line length is really one less than strlen returns, because of the
    // newline character.
    int len = strlen(first_line) - 1;

    int tree_counts[5] = {0, 0, 0, 0, 0};

    int increments[5] = {1, 3, 5, 7, 1};
    int positions[5] = {1, 3, 5, 7, 1};

    // Use this to determine if we should count this line for slope 5.
    bool tick = false;

    while (fgets(buffer, 100, fp) != NULL)
    {
        // Overwrite the newline with a null.
        buffer[len] = '\0';

        for (int i = 0; i < 5; ++i)
        {
            if (i == 4 && !tick)
            {
                continue;
            }
            int index = positions[i] % len;
            tree_counts[i] += buffer[index] == '#';
            positions[i] += increments[i];
        }
        tick = !tick;
    }

    long solution = 1;
    for (int i = 0; i < 5; ++i)
    {
        solution *= tree_counts[i];
    }

    printf("Solution: %ld\n", solution);
}