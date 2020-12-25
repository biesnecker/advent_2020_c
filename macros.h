#pragma once
#include <stdlib.h>

#define FLOG(...)                                                              \
    do {                                                                       \
        fprintf(stderr, __VA_ARGS__);                                          \
    } while (0);

#define FAIL(...)                                                              \
    do {                                                                       \
        FLOG(__VA_ARGS__);                                                     \
        exit(EXIT_FAILURE);                                                    \
    } while (0)