#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct parse_state
{
    bool has_valid_byr;
    bool has_valid_iyr;
    bool has_valid_eyr;
    bool has_valid_hgt;
    bool has_valid_hcl;
    bool has_valid_ecl;
    bool has_valid_pid;
    bool has_valid_cid;
};

static bool is_valid_state(struct parse_state *state)
{
    if (state == NULL)
    {
        return false;
    }
    return state->has_valid_byr &&
           state->has_valid_iyr &&
           state->has_valid_eyr &&
           state->has_valid_hgt &&
           state->has_valid_hcl &&
           state->has_valid_ecl &&
           state->has_valid_pid;
}

static bool is_valid_field(const char *prefix, const char *s)
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

void day_four_solution(FILE *fp)
{

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
            if (!state->has_valid_byr && is_valid_field("byr", found))
            {
                state->has_valid_byr = true;
            }
            else if (!state->has_valid_iyr && is_valid_field("iyr", found))
            {
                state->has_valid_iyr = true;
            }
            else if (!state->has_valid_eyr && is_valid_field("eyr", found))
            {
                state->has_valid_eyr = true;
            }
            else if (!state->has_valid_hgt && is_valid_field("hgt", found))
            {
                state->has_valid_hgt = true;
            }
            else if (!state->has_valid_hcl && is_valid_field("hcl", found))
            {
                state->has_valid_hcl = true;
            }
            else if (!state->has_valid_ecl && is_valid_field("ecl", found))
            {
                state->has_valid_ecl = true;
            }
            else if (!state->has_valid_pid && is_valid_field("pid", found))
            {
                state->has_valid_pid = true;
            }
            else if (!state->has_valid_cid && is_valid_field("cid", found))
            {
                state->has_valid_cid = true;
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
}