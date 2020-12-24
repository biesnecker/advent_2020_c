#ifndef __BIT_VECTOR_H__
#define __BIT_VECTOR_H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct _bitvector bitvector;

bitvector* bitvector_new(size_t capacity);

void bitvector_free(bitvector* b);

void bitvector_set(bitvector* b, size_t index);

bool bitvector_check(const bitvector* b, size_t index);

void bitvector_clear(bitvector* b);

#endif