#include <stdint.h>
#include <stdio.h>
#include <string.h>

void day_six_solution(FILE* fp) {
    char buffer[50] = {0};
    int total_sum = 0;

    uint32_t seen = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int len = strlen(buffer);
        if (len < 2) {
            // newline
            total_sum += __builtin_popcount(seen);
            seen = 0;
            continue;
        }

        for (int i = 0; i < len - 1; ++i) {
            int v = buffer[i] - 'a';
            seen |= 1L << v;
        }
    }

    // Get the last group.
    total_sum += __builtin_popcount(seen);

    printf("Solution: %d\n", total_sum);
}

void day_six_b_solution(FILE* fp) {
    char buffer[50] = {0};
    int lines = 0;
    int total_sum = 0;

    uint8_t counts[26] = {0};

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int len = strlen(buffer);
        if (len < 2) {
            // newline
            if (lines > 0) {
                for (int i = 0; i < 26; ++i) {
                    total_sum += counts[i] == lines;
                }
                lines = 0;
                memset(counts, 0, sizeof(counts));
            }
            continue;
        }

        for (int i = 0; i < len - 1; ++i) {
            int idx = buffer[i] - 'a';
            counts[idx] += 1;
        }
        ++lines;
    }

    // Get the last group.
    if (lines > 0) {
        for (int i = 0; i < 26; ++i) {
            total_sum += counts[i] == lines;
        }
        lines = 0;
    }

    printf("Solution: %d\n", total_sum);
}