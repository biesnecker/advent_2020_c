#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct parse_state
{
    bool has_byr;
    bool has_iyr;
    bool has_eyr;
    bool has_hgt;
    bool has_hcl;
    bool has_ecl;
    bool has_pid;
    bool has_cid;
};

bool is_valid_state(struct parse_state *state)
{
    if (state == NULL)
    {
        return false;
    }
    return state->has_byr &&
           state->has_iyr &&
           state->has_eyr &&
           state->has_hgt &&
           state->has_hcl &&
           state->has_ecl &&
           state->has_pid;
}

bool is_valid_field(const char *prefix, const char *s)
{
    if (strlen(s) < 5 || strlen(prefix) < 3)
    {
        return false;
    }
    return prefix[0] == s[0] &&
           prefix[1] == s[1] &&
           prefix[2] == s[2] &&
           s[3] == ':';
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

    char buffer[512] = {0};
    struct parse_state *state = NULL;
    int valid = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strlen(buffer) == 1)
        {
            if (is_valid_state(state))
            {
                ++valid;
            }
            free(state);
            state = NULL;
        }

        // Create a new state if needed.
        if (state == NULL)
        {
            state = calloc(1, sizeof(struct parse_state));
        }

        char *found = strtok(buffer, " ");
        while (found)
        {
            if (!state->has_byr && is_valid_field("byr", found))
            {
                state->has_byr = true;
            }
            else if (!state->has_iyr && is_valid_field("iyr", found))
            {
                state->has_iyr = true;
            }
            else if (!state->has_eyr && is_valid_field("eyr", found))
            {
                state->has_eyr = true;
            }
            else if (!state->has_hgt && is_valid_field("hgt", found))
            {
                state->has_hgt = true;
            }
            else if (!state->has_hcl && is_valid_field("hcl", found))
            {
                state->has_hcl = true;
            }
            else if (!state->has_ecl && is_valid_field("ecl", found))
            {
                state->has_ecl = true;
            }
            else if (!state->has_pid && is_valid_field("pid", found))
            {
                state->has_pid = true;
            }
            else if (!state->has_cid && is_valid_field("cid", found))
            {
                state->has_cid = true;
            }

            // Get the next token.
            found = strtok(NULL, " ");
        }
    }

    // Make sure if we have a state at the end of the loop that we check it.
    if (state != NULL)
    {
        if (is_valid_state(state))
        {
            ++valid;
        }
        free(state);
    }

    printf("Solution: %d\n", valid);

    fclose(fp);

    return EXIT_SUCCESS;
}