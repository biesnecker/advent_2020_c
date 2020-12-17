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
extern void day_four_b_solution(FILE *);
extern void day_five_solution(FILE *);
extern void day_five_b_solution(FILE *);
extern void day_six_solution(FILE *);
extern void day_six_b_solution(FILE *);
extern void day_seven_solution(FILE *);
extern void day_seven_b_solution(FILE *);
extern void day_eight_solution(FILE *);
extern void day_eight_b_solution(FILE *);
extern void day_nine_solution(FILE *);
extern void day_nine_b_solution(FILE *);

typedef struct _solution
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
    },
    {
        .puzzle_id = "day_four_b",
        .solution = &day_four_b_solution,
        "input/day_four_input.txt",
    },
    {
        .puzzle_id = "day_five",
        .solution = &day_five_solution,
        "input/day_five_input.txt",
    },
    {
        .puzzle_id = "day_five_b",
        .solution = &day_five_b_solution,
        "input/day_five_input.txt",
    },
    {
        .puzzle_id = "day_six",
        .solution = &day_six_solution,
        "input/day_six_input.txt",
    },
    {
        .puzzle_id = "day_six_b",
        .solution = &day_six_b_solution,
        "input/day_six_input.txt",
    },
    {
        .puzzle_id = "day_seven",
        .solution = &day_seven_solution,
        "input/day_seven_input.txt",
    },
    {
        .puzzle_id = "day_seven_b",
        .solution = &day_seven_b_solution,
        "input/day_seven_input.txt",
    },
    {
        .puzzle_id = "day_eight",
        .solution = &day_eight_solution,
        "input/day_eight_input.txt",
    },
    {
        .puzzle_id = "day_eight_b",
        .solution = &day_eight_b_solution,
        "input/day_eight_input.txt",
    },
    {
        .puzzle_id = "day_nine",
        .solution = &day_nine_solution,
        "input/day_nine_input.txt",
    },
    {
        .puzzle_id = "day_nine_b",
        .solution = &day_nine_b_solution,
        "input/day_nine_input.txt",
    }};

static int n_solutions = sizeof(solutions) / sizeof(solutions[0]);

static void print_usage(const char *program_name)
{
    FLOG("Usage: %s <puzzle_id>\n\n", program_name);
    FLOG("Available puzzles:\n");
    for (int i = 0; i < n_solutions; ++i)
    {
        FLOG("\t%s\n", solutions[i].puzzle_id);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *puzzle_id = argv[1];
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