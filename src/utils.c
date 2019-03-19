#include "utils.h"
#include <time.h>
#include <unistd.h>

void random_init() {
    const time_t current_time = time(NULL);
    const clock_t current_clock = clock();
    srand(current_time + current_clock);
}

int random_number(const int min_num, const int max_num) {
    return (rand() % (max_num + 1 - min_num)) + min_num;
}

void timer_sleep(const unsigned int milliseconds) {
    usleep(milliseconds * 1000);
}
