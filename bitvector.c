#include "bitvector.h"

#include <stdint.h>

struct _bitvector
{
    size_t capacity;
    uint8_t bytes[];
};

bitvector *bitvector_new(size_t capacity)
{
    size_t capacity_bytes = (capacity / 8) + (capacity % 8 > 0);
    size_t sz = sizeof(bitvector) + (sizeof(uint8_t) * capacity_bytes);
    bitvector *b = calloc(sz, 1);
    if (b != NULL)
    {
        b->capacity = capacity;
    }
    return b;
}

void bitvector_free(bitvector *b)
{
    free(b);
}

#define BYTE_INDEX(i) ((i) >> 3)
#define BIT_MASK(i) (1 << ((i) % 8))

bool bitarray_check(uint8_t bitarray[], int index)
{
    uint8_t byte = bitarray[BYTE_INDEX(index)];
    return byte & BIT_MASK(index);
}

void bitvector_set(bitvector *b, size_t index)
{
    size_t byte_index = BYTE_INDEX(index);
    if (byte_index > b->capacity)
    {
        return;
    }
    b->bytes[byte_index] |= BIT_MASK(index);
}

bool bitvector_check(const bitvector *b, size_t index)
{
    size_t byte_index = BYTE_INDEX(index);
    if (byte_index > b->capacity)
    {
        return false;
    }
    return b->bytes[byte_index] & BIT_MASK(index);
}
