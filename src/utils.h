#ifndef UTILS_H
#define UTILS_H

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>

void random_init();

int random_number(const int min_num, const int max_num);

void timer_sleep(const unsigned int milliseconds);

typedef enum
{
    False = 0,
    True = 1
} Bool;

#define ASSERT_ALLOC(p)                                  \
    if (p == NULL)                                       \
    {                                                    \
        fprintf(stderr, "failed to allocate memory.\n"); \
        exit(-1);                                        \
    }

#endif /* UTILS_H */
