#include "input.h"
#include <curses.h>

void input_init(WINDOW *window) {
    noecho();
    cbreak();
    nodelay(window, TRUE);
    keypad(window, TRUE);
}

GameInput input_get_key(WINDOW *window) {
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
