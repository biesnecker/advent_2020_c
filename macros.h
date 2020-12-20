#pragma once

#define FLOG(...)                     \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
    } while (0);