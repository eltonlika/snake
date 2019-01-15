#include "utils.h"
#include <unistd.h>

void game_sleep(unsigned int milliseconds) {
    usleep(milliseconds * 1000);
}
