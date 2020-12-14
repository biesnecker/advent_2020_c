#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void day_two_solution(FILE *fp)
{
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
}

void day_two_b_solution(FILE *fp)
{
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
}