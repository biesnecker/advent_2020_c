#include <stdio.h>
#include <stdlib.h>

#define ADAPTER_COUNT 112

static int cmp(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

static int read_input(FILE* fp, int* adapters) {
    adapters[0] = 0;
    int idx = 1;
    while (fscanf(fp, "%d\n", &adapters[idx]) != EOF) {
        idx += 1;
    }

    qsort(adapters, idx, sizeof(int), &cmp);
    adapters[idx] = adapters[idx - 1] + 3;
    idx += 1;

    return idx;
}

void day_ten_solution(FILE* fp) {
    int adapters[ADAPTER_COUNT] = {0};

    int ones = 0;
    int threes = 0;

    int count = read_input(fp, adapters);

    for (int i = 1; i < count + 1; ++i) {
        int diff = adapters[i] - adapters[i - 1];
        ones += diff == 1 ? 1 : 0;
        threes += diff == 3 ? 1 : 0;
    }

    printf("Solution: %d x %d = %d\n", ones, threes, ones * threes);
}

static long long
path_count(int* adapters, long long* cache, int adapter_count, int idx) {
    if (idx == adapter_count - 1) {
        return 1;
    }
    long long total = 0;
    // From the current index, the max index we can reach is three
    // higher, so don't go beyond four.
    int max_idx = idx + 4;
    // But never higher than adapter_count
    if (max_idx > adapter_count) {
        max_idx = adapter_count;
    }
    for (int i = idx + 1; i < max_idx; ++i) {
        if (adapters[i] - adapters[idx] < 4) {
            if (cache[i] == 0) {
                long long subtotal =
                    path_count(adapters, cache, adapter_count, i);
                cache[i] = subtotal;
                total += subtotal;
            } else {
                total += cache[i];
            }
        }
    }
    return total;
}

void day_ten_b_solution(FILE* fp) {
    int adapters[ADAPTER_COUNT] = {0};
    int count = read_input(fp, adapters);

    long long path_count_cache[ADAPTER_COUNT] = {0};
    long long total = path_count(adapters, path_count_cache, count, 0);

    printf("Solution: %lld\n", total);
}