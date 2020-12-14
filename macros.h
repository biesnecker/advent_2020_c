#ifndef __MACROS_H__
#define __MACROS_H__

#define FLOG(...)                     \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
    } while (0);

#endif