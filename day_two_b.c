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

    int valid = 0;

    int first_pos = 0;
    int second_pos = 0;
    char target = '\0';
    char password[100] = {0};

    while (fscanf(fp, "%d-%d %c: %s\n", &first_pos, &second_pos, &target, password) != EOF)
    {
        bool first_found = false;
        bool second_found = false;

        // zero index the locations
        first_pos -= 1;
        second_pos -= 1;

        int len = strlen(password);
        if (first_pos < len)
        {
            first_found = password[first_pos] == target;
        }
        if (second_pos < len)
        {
            second_found = password[second_pos] == target;
        }
        valid += first_found ^ second_found;
    }

    printf("Solution: %d\n", valid);
    fclose(fp);

    return EXIT_SUCCESS;
}