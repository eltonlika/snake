#include "renderer.h"
#include <curses.h>
#include <stdlib.h>

Screen *renderer_init() {
    Screen *screen = malloc(sizeof(Screen));
    WINDOW *main_window = initscr();
    unsigned int width, height;

    curs_set(0); /* set cursor invisible */
    werase(main_window);
    wrefresh(main_window);
    getmaxyx(main_window, height, width);

    /* set playfield width and height smaller to account for window borders */
    screen->width = width - 2;
    screen->height = height - 2;
    screen->main_window = main_window;
    return screen;
}

void renderer_render(Screen *screen, Game *game) {
    const Snake *snake = &game->snake;
    const Position *cells = snake->cells;
    const Position head = cells[0];
    const Direction direction = snake->direction;
    const unsigned int length = snake->length;
    const char food_character = 'X';
    const char body_character = '#';
    char head_character;
    WINDOW *window = screen->main_window;
    Position food_pos, cell_pos;
    unsigned int idx;

    werase(window);

    /* render window border */
    box(window, 0, 0);

    /* render tail */
    for (idx = 1; idx < length; idx++) {
        cell_pos = cells[idx];
        /* +1 coordinates to account for window border */
        mvwaddch(window, cell_pos.y + 1, cell_pos.x + 1, body_character);
    }

    /* render snake head */
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
    /* +1 coordinates to account for window border */
    mvwaddch(window, head.y + 1, head.x + 1, head_character);

    /* render food */
    food_pos = game->food;
    /* +1 coordinates to account for window border */
    mvwaddch(window, food_pos.y + 1, food_pos.x + 1, food_character);

    /* refresh window to draw new renderings */
    wrefresh(window);
}

void renderer_end(Screen *screen) {
    werase(screen->main_window);
    endwin();
}
