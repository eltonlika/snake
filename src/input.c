#include "input.h"
#include <curses.h>

void input_init(WINDOW *window) {
    noecho();
    cbreak();
    nodelay(window, TRUE);
    keypad(window, TRUE);
}

GameInput input_get_next_key(WINDOW *window) {
    const int c = wgetch(window);

    switch (c) {
    case ERR:
        return NoInput;
    case KEY_UP:
        return KeyUp;
    case KEY_RIGHT:
        return KeyRight;
    case KEY_DOWN:
        return KeyDown;
    case KEY_LEFT:
        return KeyLeft;
    case 'p':
        return KeyPause;
    case 'P':
        return KeyPause;
    case 'q':
        return KeyQuit;
    case 'Q':
        return KeyQuit;
    case 'n':
        return KeyNewGame;
    case 'N':
        return KeyNewGame;
    case 's':
        return KeySpeedIncrease;
    case 'S':
        return KeySpeedDecrease;
    }

    return NoInput;
}

GameInput input_get_last_key(WINDOW *window) {
    GameInput last_key = NoInput, read_key;

    while ((read_key = input_get_next_key(window)) != NoInput) {
        last_key = read_key;
    }

    return last_key;
}
