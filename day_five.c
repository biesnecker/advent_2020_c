#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "bitvector.h"
#include "macros.h"

static bool get_row_and_seat(const char *code, int *row, int *seat)
{
    int row_min = 0;
    int row_max = 127;
    int seat_min = 0;
    int seat_max = 7;
    int half = 0;
    for (int i = 0; i < 10; ++i)
    {
        char c = code[i];
        if (i < 7 && (c == 'F' || c == 'B'))
        {
            half = (row_max - row_min + 1) / 2;
            if (c == 'F')
            {
                row_max -= half;
            }
            else
            {
                row_min += half;
            }
        }
        else if (i > 6 && (c == 'L' || c == 'R'))
        {
            half = (seat_max - seat_min + 1) / 2;
            if (c == 'L')
            {
                seat_max -= half;
            }
            else
            {
                seat_min += half;
            }
        }
        else
        {
            return false;
        }
    }
    // The min and max for each should be the same
    if (row_max != row_min || seat_max != seat_min)
    {
        return false;
    }

    *row = row_max;
    *seat = seat_max;
    return true;
}

void day_five_solution(FILE *fp)
{
    char buffer[16] = {0};
    int row = 0;
    int seat = 0;
    int max_seat_code = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strlen(buffer) != 11)
        {
            // Sanity.
            continue;
        }
        if (!get_row_and_seat(buffer, &row, &seat))
        {
            FLOG("Invalid code: %s", buffer);
            return;
        }
        int seat_code = (row * 8) + seat;
        if (seat_code > max_seat_code)
        {
            max_seat_code = seat_code;
        }
    }
    printf("Max seat code: %d\n", max_seat_code);
}

void day_five_b_solution(FILE *fp)
{
    char buffer[16] = {0};
    int row = 0;
    int seat = 0;

    bitvector *bv = bitvector_new(128 * 8);
    if (bv == NULL)
    {
        FLOG("Couldn't create bit vector");
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strlen(buffer) != 11)
        {
            // Sanity.
            continue;
        }
        if (!get_row_and_seat(buffer, &row, &seat))
        {
            FLOG("Invalid code: %s", buffer);
            goto cleanup;
        }
        int seat_code = (row * 8) + seat;
        bitvector_set(bv, seat_code);
    }

    // Loop through, skipping the first and last seats. If we find a hole with both
    // seats filled next to it then it will be ours.
    for (int i = 1; i < (128 * 8) - 1; ++i)
    {
        if (
            bitvector_check(bv, i - 1) &&
            bitvector_check(bv, i + 1) &&
            !bitvector_check(bv, i))
        {
            printf("We're at seat #%d\n", i);
            goto cleanup;
        }
    }

cleanup:
    bitvector_free(bv);
}