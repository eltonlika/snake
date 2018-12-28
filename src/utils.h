#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <time.h>

typedef unsigned int uint;

typedef enum { false = 0, true = 1 } bool;

void init_random_generator();

int random_number(int min_num, int max_num);

#endif /* UTILS_H */
