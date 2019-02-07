#include "utils.h"
#include <time.h>
#include <unistd.h>

void random_init() {
    time_t current_time = time(NULL);
    clock_t current_clock = clock();
    srand(current_time + current_clock);
}

int random_number(int min_num, int max_num) {
    return (rand() % (max_num + 1 - min_num)) + min_num;
}

void timer_sleep(unsigned int milliseconds) {
    usleep(milliseconds * 1000);
}
