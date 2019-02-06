#include "input.h"
#include "utils.h"
#include <curses.h>

void input_init(WINDOW *window) {
    noecho();
    cbreak();
    nodelay(window, TRUE);
    keypad(window, TRUE);
}

GameInput input_get_next_key(WINDOW *window) {
    const int input = wgetch(window);

    switch (input) {
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
    case 'P':
        return KeyPause;
    case 'q':
    case 'Q':
        return KeyQuit;
    case 'n':
    case 'N':
        return KeyNewGame;
    case 's':
        return KeySpeedIncrease;
    case 'S':
        return KeySpeedDecrease;
    default:
        return NoInput;
    }
}

Bool input_is_snake_control(GameInput input) {
    switch (input) {
    case KeyUp:
    case KeyRight:
    case KeyDown:
    case KeyLeft:
        return True;
    default:
        return False;
    }
}
