#include "renderer.h"
#include <curses.h>

WINDOW *renderer_init() {
    WINDOW *window = initscr();
    noecho();
    cbreak();
    curs_set(0); /* set cursor invisible */
    nodelay(window, TRUE);
    keypad(window, TRUE);
    werase(window);
    return window;
}

void renderer_render(WINDOW *window, Game *game) {
    const Snake *snake = &game->snake;
    const Position *cells = snake->cells;
    const Position head = cells[0];
    const Direction direction = snake->direction;
    const unsigned int length = snake->length;
    const char food_character = 'X';
    const char body_character = '#';
    char head_character;
    Position food_pos, cell_pos;

    unsigned int idx;

    werase(window);

    /* print tail */
    for (idx = 1; idx < length; idx++) {
        cell_pos = cells[idx];
        mvwaddch(window, cell_pos.y, cell_pos.x, body_character);
    }

    /* print head */
    switch (direction) {
    case DirectionUp:
        head_character = 'A';
        break;
    case DirectionDown:
        head_character = 'V';
        break;
    case DirectionLeft:
        head_character = '<';
        break;
    case DirectionRight:
        head_character = '>';
        break;
    }
    mvwaddch(window, head.y, head.x, head_character);

    /* print food */
    food_pos = game->food;
    mvwaddch(window, food_pos.y, food_pos.x, food_character);

    wrefresh(window);
}

unsigned int renderer_get_max_width(WINDOW *window) { return getmaxx(window); }

unsigned int renderer_get_max_height(WINDOW *window) { return getmaxy(window); }

void renderer_end(WINDOW *window) {
    werase(window);
    endwin();
}
