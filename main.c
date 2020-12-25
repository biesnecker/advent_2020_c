#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

// Extern definitions of solution functions go here.

#define FUNCTIONS_FOR_DAY(day_name)                                            \
    extern void day_##day_name##_solution(FILE*);                              \
    extern void day_##day_name##_b_solution(FILE*)

FUNCTIONS_FOR_DAY(one);
FUNCTIONS_FOR_DAY(two);
FUNCTIONS_FOR_DAY(three);
FUNCTIONS_FOR_DAY(four);
FUNCTIONS_FOR_DAY(five);
FUNCTIONS_FOR_DAY(six);
FUNCTIONS_FOR_DAY(seven);
FUNCTIONS_FOR_DAY(eight);
FUNCTIONS_FOR_DAY(nine);
FUNCTIONS_FOR_DAY(ten);
FUNCTIONS_FOR_DAY(eleven);
FUNCTIONS_FOR_DAY(twelve);
FUNCTIONS_FOR_DAY(thirteen);
FUNCTIONS_FOR_DAY(fourteen);
FUNCTIONS_FOR_DAY(fifteen);
FUNCTIONS_FOR_DAY(sixteen);
FUNCTIONS_FOR_DAY(seventeen);
FUNCTIONS_FOR_DAY(eighteen);
FUNCTIONS_FOR_DAY(twentytwo);

typedef struct _solution {
    const char* puzzle_id;
    void (*solution)(FILE*);
    const char* input_filename;
} solution;

#define SOLUTIONS_FOR_DAY(day_name)                                            \
    {                                                                          \
        .puzzle_id = "day_" #day_name,                                         \
        .solution = &day_##day_name##_solution,                                \
        "input/day_" #day_name "_input.txt",                                   \
    },                                                                         \
    {                                                                          \
        .puzzle_id = "day_" #day_name "_b",                                    \
        .solution = &day_##day_name##_b_solution,                              \
        "input/day_" #day_name "_input.txt",                                   \
    }

static solution solutions[] = {SOLUTIONS_FOR_DAY(one),
                               SOLUTIONS_FOR_DAY(two),
                               SOLUTIONS_FOR_DAY(three),
                               SOLUTIONS_FOR_DAY(four),
                               SOLUTIONS_FOR_DAY(five),
                               SOLUTIONS_FOR_DAY(six),
                               SOLUTIONS_FOR_DAY(seven),
                               SOLUTIONS_FOR_DAY(eight),
                               SOLUTIONS_FOR_DAY(nine),
                               SOLUTIONS_FOR_DAY(ten),
                               SOLUTIONS_FOR_DAY(eleven),
                               SOLUTIONS_FOR_DAY(twelve),
                               SOLUTIONS_FOR_DAY(thirteen),
                               SOLUTIONS_FOR_DAY(fourteen),
                               SOLUTIONS_FOR_DAY(fifteen),
                               SOLUTIONS_FOR_DAY(sixteen),
                               SOLUTIONS_FOR_DAY(seventeen),
                               SOLUTIONS_FOR_DAY(eighteen),
                               SOLUTIONS_FOR_DAY(twentytwo)};

static int n_solutions = sizeof(solutions) / sizeof(solutions[0]);

static void print_usage(const char* program_name) {
    FLOG("Usage: %s <puzzle_id>\n\n", program_name);
    FLOG("Available puzzles:\n");
    for (int i = 0; i < n_solutions; ++i) {
        FLOG("\t%s\n", solutions[i].puzzle_id);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char* puzzle_id = argv[1];
    for (int i = 0; i < n_solutions; ++i) {
        if (strcmp(puzzle_id, solutions[i].puzzle_id) == 0) {
            const char* filename = solutions[i].input_filename;
            FILE* fp = fopen(filename, "r");
            if (fp == NULL) {
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