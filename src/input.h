#ifndef INPUT_H
#define INPUT_H

#include <curses.h>

typedef enum {
    NoInput = 0,
    KeyPause,
    KeyQuit,
    KeyUp,
    KeyRight,
    KeyDown,
    KeyLeft
} GameInput;

GameInput input_get_key(WINDOW *source);

#endif /* INPUT_H */
