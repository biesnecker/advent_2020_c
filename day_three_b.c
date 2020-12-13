#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    char buffer[100] = {0};

    char *first_line = fgets(buffer, 100, fp);
    if (first_line == NULL)
    {
        fclose(fp);
        fprintf(stderr, "First line was empty?!\n");
        return EXIT_FAILURE;
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

    fclose(fp);

    return EXIT_SUCCESS;
}