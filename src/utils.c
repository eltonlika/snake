#include "utils.h"

void init_random_generator() {
    time_t current_time = time(NULL);
    clock_t current_clock = clock();
    srand(current_time ^ current_clock);
}

int random_number(int min_num, int max_num) {
    int low_num, hi_num;

    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num + 1; /* include max_num in output */
    } else {
        low_num = max_num + 1; /* include max_num in output */
        hi_num = min_num;
    }

    return (rand() % (hi_num - low_num)) + low_num;
}
