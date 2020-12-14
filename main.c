#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

// Extern definitions of solution functions go here.
extern void day_one_solution(FILE *);
extern void day_one_b_solution(FILE *);
extern void day_two_solution(FILE *);
extern void day_two_b_solution(FILE *);
extern void day_three_solution(FILE *);
extern void day_three_b_solution(FILE *);
extern void day_four_solution(FILE *);

typedef struct solutions
{
    const char *puzzle_id;
    void (*solution)(FILE *);
    const char *input_filename;
} solution;

static solution solutions[] = {
    {
        .puzzle_id = "day_one",
        .solution = &day_one_solution,
        "input/day_one_input.txt",
    },
    {
        .puzzle_id = "day_one_b",
        .solution = &day_one_b_solution,
        "input/day_one_input.txt",
    },
    {
        .puzzle_id = "day_two",
        .solution = &day_two_solution,
        "input/day_two_input.txt",
    },
    {
        .puzzle_id = "day_two_b",
        .solution = &day_two_b_solution,
        "input/day_two_input.txt",
    },
    {
        .puzzle_id = "day_three",
        .solution = &day_three_solution,
        "input/day_three_input.txt",
    },
    {
        .puzzle_id = "day_three_b",
        .solution = &day_three_b_solution,
        "input/day_three_input.txt",
    },
    {
        .puzzle_id = "day_four",
        .solution = &day_four_solution,
        "input/day_four_input.txt",
    }};

static void print_usage(const char *program_name)
{
    FLOG("Usage: %s <puzzle_id>\n\n", program_name);
    FLOG(
        "Available puzzles:\n"
        "\tday_one\n"
        "\tday_one_b\n"
        "\tday_two\n"
        "\tday_two_b\n"
        "\tday_three\n"
        "\tday_three_b\n"
        "\tday_four\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *puzzle_id = argv[1];
    int n_solutions = sizeof(solutions) / sizeof(solutions[0]);
    for (int i = 0; i < n_solutions; ++i)
    {
        if (strcmp(puzzle_id, solutions[i].puzzle_id) == 0)
        {
            const char *filename = solutions[i].input_filename;
            FILE *fp = fopen(filename, "r");
            if (fp == NULL)
            {
                fprintf(stderr, "Unable to open file: %s\n", filename);
                return EXIT_FAILURE;
            }
            solutions[i].solution(fp);
            fclose(fp);
            return EXIT_SUCCESS;
        }
    }

    FLOG("Invalid puzzle_id: %s\n", argv[1]);
    print_usage(argv[0]);
    return EXIT_FAILURE;
}