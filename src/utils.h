#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    False = 0,
    True = 1
} Bool;

#define ASSERT_ALLOC(p)                                  \
    if (p == NULL) {                                     \
        fprintf(stderr, "failed to allocate memory.\n"); \
        exit(-1);                                        \
    }

#endif /* UTILS_H */
