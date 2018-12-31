#ifndef INPUT_H
#define INPUT_H

#include <curses.h>

typedef enum {
    NoInput = 0,
    KeyNewGame,
    KeyPause,
    KeyQuit,
    KeySpeedIncrease,
    KeySpeedDecrease,
    KeyUp,
    KeyRight,
    KeyDown,
    KeyLeft
} GameInput;

void input_init(WINDOW *window);

GameInput input_get_key(WINDOW *window);

#endif /* INPUT_H */
