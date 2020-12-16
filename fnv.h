#ifndef __FNV_H__
#define __FNV_H__

#include <stdint.h>
#include <stdlib.h>

uint32_t fnv32(const void *data, size_t len);

uint32_t fnv32_char(const char *s);

#endif