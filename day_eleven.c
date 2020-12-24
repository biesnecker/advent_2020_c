#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

#define MAX_ROWS 100
#define MAX_COLS 100

typedef char seating_chart[MAX_ROWS][MAX_COLS];

// Order: N NE E SE S SW W NW
static const int col_offset[8] = {0, 1, 1, 1, 0, -1, -1, -1};
static const int row_offset[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

static int read_input(FILE* fp,
                      seating_chart buffer_a,
                      seating_chart buffer_b,
                      int* rows,
                      int* columns) {
    int bytes_read = 0;
    char line_buffer[MAX_COLS] = {0};
    *rows = 0;
    *columns = 0;
    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        // Don't copy the newline
        int len = strlen(line_buffer) - 1;
        if (*columns == 0) {
            *columns = len;
        } else if (*columns != len) {
            FLOG("Mismatched line lengths.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(buffer_a[*rows], line_buffer, len);
        memcpy(buffer_b[*rows], line_buffer, len);
        *rows += 1;
        bytes_read += len;
    }
    return bytes_read;
}

static int get_los_occupied(seating_chart seats,
                            int row_idx,
                            int col_idx,
                            int rows,
                            int cols) {
    int occupied = 0;
    int adjacent_row, adjacent_column;

    for (int i = 0; i < 8; ++i) {
        adjacent_column = col_idx;
        adjacent_row = row_idx;
        bool stop = false;
        do {
            adjacent_column += col_offset[i];
            adjacent_row += row_offset[i];
            if (adjacent_column < 0 || adjacent_column >= cols ||
                adjacent_row < 0 || adjacent_row >= rows) {
                // out of bounds
                stop = true;
                continue;
            }
            char seat = seats[adjacent_row][adjacent_column];
            if (seat == '.') {
                continue;
            }
            occupied += seat == '#';
            stop = true;
        } while (!stop);
    }
    return occupied;
}

static int get_adjacent_occupied(seating_chart seats,
                                 int row_idx,
                                 int col_idx,
                                 int rows,
                                 int cols) {
    int occupied = 0;
    int adjacent_row, adjacent_column;

    for (int i = 0; i < 8; ++i) {
        adjacent_column = col_idx + col_offset[i];
        adjacent_row = row_idx + row_offset[i];
        if (adjacent_column < 0 || adjacent_column >= cols ||
            adjacent_row < 0 || adjacent_row >= rows) {
            continue;
        }
        occupied += seats[adjacent_row][adjacent_column] == '#';
    }
    return occupied;
}

static int game_step(seating_chart current,
                     seating_chart previous,
                     int rows,
                     int columns,
                     bool puzzle_two) {
    const int occupied_threshold = puzzle_two ? 4 : 3;
    int total_occupied = 0;
    int occupied_count = 0;
    char previous_state;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            previous_state = previous[i][j];
            if (previous_state == '.') {
                continue;
            }
            occupied_count =
                puzzle_two
                    ? get_los_occupied(previous, i, j, rows, columns)
                    : get_adjacent_occupied(previous, i, j, rows, columns);
            if (previous_state == 'L') {
                if (occupied_count == 0) {
                    current[i][j] = '#';
                    ++total_occupied;
                } else {
                    current[i][j] = 'L';
                }
            } else if (previous_state == '#') {
                if (occupied_count > occupied_threshold) {
                    current[i][j] = 'L';
                } else {
                    current[i][j] = '#';
                    ++total_occupied;
                }
            }
        }
    }
    return total_occupied;
}

static bool compare_boards(seating_chart board_a,
                           seating_chart board_b,
                           int rows,
                           int cols) {
    for (int i = 0; i < rows; ++i) {
        if (memcmp(board_a[i], board_b[i], cols) != 0) {
            return false;
        }
    }
    return true;
}

static void run_game(FILE* fp, bool puzzle_two) {
    seating_chart board_a = {0};
    seating_chart board_b = {0};

    int columns;
    int rows;
    bool tock = true;

    read_input(fp, board_a, board_b, &rows, &columns);

    int occupied = 0;

    do {
        if (tock) {
            occupied = game_step(board_a, board_b, rows, columns, puzzle_two);
        } else {
            occupied = game_step(board_b, board_a, rows, columns, puzzle_two);
        }
        tock = !tock;
    } while (!compare_boards(board_a, board_b, rows, columns));

    printf("Solution: %d\n", occupied);
}

void day_eleven_solution(FILE* fp) { run_game(fp, false /* puzzle two */); }

void day_eleven_b_solution(FILE* fp) { run_game(fp, true /* puzzle two */); }