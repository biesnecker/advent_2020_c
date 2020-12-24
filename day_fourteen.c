#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fnv.h"
#include "macros.h"

#define HASH_TABLE_EXP 17
#define HASH_TABLE_SIZE (1L << HASH_TABLE_EXP)
#define BIT_MASK ((1UL << 36) - 1)

typedef struct _memloc {
    uint64_t value;
    uint64_t location;
} memloc;

static int
create_masks(const char* mstr, uint64_t* or_mask, uint64_t* and_mask) {
    uint64_t _or_mask = 0;
    uint64_t _and_mask = 0;
    int floating_count = 0;
    for (int i = 0; i < 36; ++i) {
        _or_mask <<= 1;
        _and_mask <<= 1;
        if (mstr[i] == 'X') {
            ++floating_count;
        } else if (mstr[i] == '1') {
            _or_mask |= 1U;
        } else if (mstr[i] == '0') {
            _and_mask |= 1U;
        }
    }
    *or_mask = _or_mask;
    *and_mask = ~_and_mask & BIT_MASK;
    return floating_count;
}

static uint32_t hash_location(uint64_t location) {
    uint32_t hsh = fnv32(&location, sizeof(location));
    return (hsh >> (32 - HASH_TABLE_EXP)) ^ (hsh & (HASH_TABLE_SIZE - 1));
}

static void
update_or_insert(memloc* memory, uint64_t location, uint64_t value) {
    uint32_t hsh = hash_location(location);
    uint32_t first_hsh = hsh;
    bool loop_check = false;
    while (true) {
        if (memory[hsh].location == 0) {
            // Insert here.
            memory[hsh].location = location;
            memory[hsh].value = value;
            break;
        } else if (memory[hsh].location == location) {
            // Update here.
            memory[hsh].value = value;
            break;
        } else {
            hsh = (hsh + 1) % HASH_TABLE_SIZE;
            if (loop_check && hsh == first_hsh) {
                FLOG("Hash table is full. :-(\n");
                exit(1);
            }
            loop_check = true;
        }
    }
}

// Make the initial mask and count the number of Xs in the mask
// string.
static unsigned make_mask_count_floating(const char* mstr, uint64_t* m) {
    unsigned floating = 0;
    uint64_t mask = 0;
    for (int i = 0; i < 36; ++i) {
        mask <<= 1;
        if (mstr[i] == '1') {
            mask |= 1U;
        } else if (mstr[i] == 'X') {
            ++floating;
        }
    }
    *m = mask;
    return floating;
}

static void permute_masks(const char* mstr,
                          const char* remaining,
                          uint64_t* masks,
                          uint64_t base,
                          unsigned idx,
                          unsigned pcount) {
    if (pcount == 1) {
        masks[idx] = base;
        return;
    }

    // Split the remaining permutations in half.
    unsigned half = pcount >> 1;
    unsigned lower = idx;
    unsigned upper = idx + half;

    uint64_t base_one = base;

    char* next_x = strchr(remaining, 'X');
    base_one |= (1UL << (35 - (next_x - mstr)));

    permute_masks(mstr, next_x + 1, masks, base, lower, half);
    permute_masks(mstr, next_x + 1, masks, base_one, upper, half);
}

void day_fourteen_solution(FILE* fp) {
    memloc memory[HASH_TABLE_SIZE] = {0};
    char buffer[100] = {0};
    uint64_t or_mask = 0;
    uint64_t and_mask = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "mask = ") == buffer) {
            char* mask_start = buffer + 7;
            create_masks(mask_start, &or_mask, &and_mask);
        } else {
            char* addr_start = buffer + 4;
            long addrl = atol(addr_start);
            assert(addrl > 0);
            uint64_t addr = (uint64_t)addrl;

            char* val_start = strchr(addr_start, '=') + 1;
            long vall = atol(val_start);
            assert(vall > 0);
            uint64_t value = (uint64_t)vall;

            value |= or_mask;
            value &= and_mask;
            update_or_insert(memory, addr, value);
        }
    }

    uint64_t sum = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        if (memory[i].location != 0) {
            sum += memory[i].value;
        }
    }

    printf("Solution: %" PRIu64 "\n", sum);
}

void day_fourteen_b_solution(FILE* fp) {
    memloc memory[HASH_TABLE_SIZE] = {0};
    char buffer[100] = {0};

    uint64_t* masks = NULL;
    uint64_t address_mask = 0;
    unsigned pcount = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "mask = ") == buffer) {
            char* mask_start = buffer + 7;
            // Kill the newline.
            mask_start[36] = '\0';
            pcount = 1U << make_mask_count_floating(mask_start, &address_mask);

            free(masks);
            masks = calloc(pcount, sizeof(uint64_t));

            permute_masks(mask_start, mask_start, masks, 0, 0, pcount);
        } else {
            char* addr_start = buffer + 4;
            long addrl = atol(addr_start);
            assert(addrl > 0);
            uint64_t addr = (uint64_t)addrl;

            char* val_start = strchr(addr_start, '=') + 2;
            long vall = atol(val_start);
            assert(vall > 0);
            uint64_t value = (uint64_t)vall;

            // Apply the initial mask
            addr |= address_mask;

            // Zero out the bits that have an X. The last of the masks is always
            // the all-ones one so use its inverse.
            addr &= (~masks[pcount - 1] & BIT_MASK);

            for (unsigned i = 0; i < pcount; ++i) {
                update_or_insert(memory, addr | masks[i], value);
            }
        }
    }

    free(masks);

    uint64_t sum = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        if (memory[i].location != 0) {
            sum += memory[i].value;
        }
    }

    printf("Solution: %" PRIu64 "\n", sum);
}