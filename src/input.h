#ifndef INPUT_H
#define INPUT_H

#include "utils.h"
#include <curses.h>

typedef enum {
    NoInput = 0,
    KeyUp,
    KeyRight,
    KeyDown,
    KeyLeft,
    KeyNewGame,
    KeyPause,
    KeyQuit,
    KeySpeedIncrease,
    KeySpeedDecrease,
    KeySpeedReset,
    KeyToggleWalls
} GameInput;

void input_init(WINDOW *window);

GameInput input_get_next_key(WINDOW *window);

Bool input_is_snake_control(GameInput input);

#endif /* INPUT_H */
