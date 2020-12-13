#include <stdio.h>
#include <stdlib.h>

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

    int valid = 0;

    int min_count = 0;
    int max_count = 0;
    char target = '\0';
    char password[100] = {0};

    while (fscanf(fp, "%d-%d %c: %s\n", &min_count, &max_count, &target, password) != EOF)
    {
        int count = 0;
        char *p;
        for (p = password; *p != '\0'; ++p)
        {
            if (*p == target)
            {
                ++count;
            }
        }
        if ((count >= min_count) && (count <= max_count))
        {
            ++valid;
        }
    }

    printf("Solution: %d\n", valid);
    fclose(fp);

    return EXIT_SUCCESS;
}