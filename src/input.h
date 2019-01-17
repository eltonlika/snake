#ifndef INPUT_H
#define INPUT_H

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
    KeySpeedDecrease
} GameInput;

void input_init(WINDOW *window);

GameInput input_get_next_key(WINDOW *window);

#endif /* INPUT_H */
