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

    printf("Tree encountered: %d\n", tree_count);

    fclose(fp);

    return EXIT_SUCCESS;
}