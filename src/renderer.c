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
    const unsigned int length = snake->length;
    Position food_pos, cell_pos;
    unsigned int idx;

    werase(window);

    for (idx = 0; idx < length; idx++) {
        cell_pos = cells[idx];
        mvwaddch(window, cell_pos.y, cell_pos.x, '#');
    }

    food_pos = game->food;
    mvwaddch(window, food_pos.y, food_pos.x, 'o');

    wrefresh(window);
}

unsigned int renderer_get_max_width(WINDOW *window) { return getmaxx(window); }

unsigned int renderer_get_max_height(WINDOW *window) { return getmaxy(window); }

void renderer_end(WINDOW *window) {
    werase(window);
    endwin();
}
