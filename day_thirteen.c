#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void day_thirteen_solution(FILE* fp) {
    char buffer[150] = {0};
    fgets(buffer, sizeof(buffer), fp);
    int target = atoi(buffer);

    fgets(buffer, sizeof(buffer), fp);
    int min_diff = INT_MAX;
    int min_bus_id = 0;
    char* next = strtok(buffer, ",");

    while (next) {
        if (next[0] != 'x') {
            int bus_id = atoi(next);
            int diff = bus_id - (target % bus_id);
            if (diff < min_diff) {
                min_diff = diff;
                min_bus_id = bus_id;
            }
        }
        next = strtok(NULL, ",");
    }
    printf("Solution: %d x %d = %d\n",
           min_bus_id,
           min_diff,
           min_bus_id * min_diff);
}

void day_thirteen_b_solution(FILE* fp) {
    char buffer[150] = {0};
    // Ignore the first line.
    fgets(buffer, sizeof(buffer), fp);

    fgets(buffer, sizeof(buffer), fp);
    int offset = 0;
    int idx = 0;
    char* next = strtok(buffer, ",");

    int buses[25] = {0};
    int offsets[25] = {0};

    while (next) {
        if (next[0] != 'x') {
            int bus_id = atoi(next);
            buses[idx] = bus_id;
            offsets[idx] = offset;
            ++idx;
        }
        ++offset;
        next = strtok(NULL, ",");
    }

    long target = 0;
    long step = 1;

    for (int i = 0; i < idx; ++i) {
        while ((target + offsets[i]) % buses[i] != 0) {
            target += step;
        }
        step *= buses[i];
    }

    printf("Solution: %ld\n", target);
}