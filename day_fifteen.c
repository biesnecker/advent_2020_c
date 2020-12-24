#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fnv.h"
#include "macros.h"

#define NUM_ROUNDS 2020

#define HASH_TABLE_EXP 22
#define HASH_TABLE_SIZE (1L << HASH_TABLE_EXP)

typedef struct _entry {
    long rnd;
    long num;
    struct _entry* next;
} entry;

static entry* new_entry(long rnd, long num) {
    entry* e = malloc(sizeof(entry));
    e->rnd = rnd;
    e->num = num;
    e->next = NULL;
    return e;
}

static long update_and_return_last_round(entry** entries, long rnd, long num) {
    uint32_t hsh = fnv32(&num, sizeof(num));
    hsh =
        (hsh >> (32 - HASH_TABLE_EXP) ^ (hsh & 0xFFFF)) & (HASH_TABLE_SIZE - 1);

    entry** tail_ptr = &entries[hsh];
    entry* e = entries[hsh];
    while (e != NULL) {
        if (e->num == num) {
            long r = e->rnd;
            e->rnd = rnd;
            return r;
        } else {
            tail_ptr = &e->next;
            e = e->next;
        }
    }

    // If we're here then we didn't find anything.
    *tail_ptr = new_entry(rnd, num);
    return rnd;
}

static entry* read_input(FILE* fp) {
    char buffer[100] = {0};

    entry* e = NULL;
    entry** ptr = &e;

    fgets(buffer, sizeof(buffer), fp);
    char* next = buffer - 1;
    do {
        next += 1;
        long i = atol(next);

        entry* ne = new_entry(0, i);

        *ptr = ne;
        ptr = &ne->next;

        next = strchr(next, ',');
    } while (next);

    return e;
}

static void release_entries(entry** entries) {
    for (long i = 0; i < HASH_TABLE_SIZE; ++i) {
        entry* e = entries[i];
        while (e) {
            entry* next = e->next;
            free(e);
            e = next;
        }
    }
}

static void play_game(FILE* fp, long rounds) {
    entry** entries = calloc(HASH_TABLE_SIZE, sizeof(entry*));

    entry* first = read_input(fp);
    entry* input = first->next;

    long rnd = 1;
    long last_num = first->num;
    long prev_rnd = 0;
    while (rnd < rounds) {
        prev_rnd = update_and_return_last_round(entries, rnd - 1, last_num);
        if (input != NULL) {
            // Add the input and ignore the previous round
            last_num = input->num;
            update_and_return_last_round(entries, rnd++, last_num);
            input = input->next;
        } else if (prev_rnd == rnd - 1) {
            // Last time the number was said was the first time it had been.
            last_num = 0;
            ++rnd;
        } else {
            last_num = rnd - 1 - prev_rnd;
            ++rnd;
        }
    }
    printf("Solution: %ld\n", last_num);
    release_entries(entries);
}

void day_fifteen_solution(FILE* fp) { play_game(fp, 2020); }

void day_fifteen_b_solution(FILE* fp) { play_game(fp, 30000000); }