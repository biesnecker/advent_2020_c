#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define PREAMBLE_LENGTH 25

static bool find_match(long long target, long long* candidates) {
    for (int i = 0; i < PREAMBLE_LENGTH; i++) {
        for (int j = 0; j < PREAMBLE_LENGTH; j++) {
            if (candidates[i] == candidates[j])
                continue;
            if (candidates[i] + candidates[j] == target) {
                return true;
            }
        }
    }
    return false;
}

void day_nine_solution(FILE* fp) {
    long long lookback[PREAMBLE_LENGTH] = {0};
    long long num = 0;
    int idx = 0;
    while (fscanf(fp, "%lld\n", &num) != EOF) {
        if (idx < PREAMBLE_LENGTH) {
            // Still in the preamble.
            lookback[idx] = num;
            idx += 1;
            continue;
        }

        if (!find_match(num, lookback)) {
            printf("Solution: %lld\n", num);
            break;
        }
        int dropped = idx % PREAMBLE_LENGTH;
        lookback[dropped] = num;

        idx += 1;
    }
}

static void find_high_low_sum(long long* numbers, int low_idx, int high_idx) {
    // Found it.
    long long min = LLONG_MAX;
    long long max = LLONG_MIN;
    for (int j = low_idx; j < high_idx + 1; ++j) {
        if (numbers[j] < min) {
            min = numbers[j];
        } else if (numbers[j] > max) {
            max = numbers[j];
        }
    }
    printf("Solution: %lld + %lld = %lld\n", min, max, min + max);
}

void day_nine_b_solution(FILE* fp) {
    long long numbers[1000] = {0};
    long long target = 41682220;
    // long long target = 127;
    long long num = 0;
    int idx = 0;
    while (fscanf(fp, "%lld\n", &num) != EOF) {
        numbers[idx++] = num;
    }

    long long sum = numbers[0];
    int low_idx = 0;

    for (int i = 1; i < 1000; ++i) {
        sum += numbers[i];
        if (sum < target) {
            // Haven't gotten there yet.
            continue;
        } else if (sum == target && low_idx < i) {
            find_high_low_sum(numbers, low_idx, i);
            return;
        } else if (sum > target) {
            // The sum is greater than the target, remove the trailing numbers
            // until it's less.
            while (sum > target) {
                sum -= numbers[low_idx++];
            }
            if (sum == target && low_idx < i) {
                find_high_low_sum(numbers, low_idx, i);
                return;
            }
        }
    }
}