#include "renderer.h"
#include "utils.h"
#include <curses.h>
#include <stdlib.h>

Renderer *renderer_init() {
    Renderer *renderer;
    WINDOW *main_window;
    unsigned int width, height;

    renderer = malloc(sizeof(Renderer));
    ASSERT_ALLOC(renderer);

    main_window = initscr();
    curs_set(0); /* set cursor invisible */
    werase(main_window);
    wrefresh(main_window);
    getmaxyx(main_window, height, width);

    /* set playfield width and height smaller to account for window borders */
    renderer->width = width - 2;
    renderer->height = height - 2;
    renderer->main_window = main_window;
    return renderer;
}

static const char food_character = 'X';
static const char body_character = '#';

void renderer_render(Renderer *renderer, Game *game) {
    const Snake *snake = &game->snake;
    const Position *cells = snake->cells;
    const Position head = cells[0];
    const Direction direction = snake->direction;
    const unsigned int length = snake->length;
    char head_character;
    WINDOW *window = renderer->main_window;
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

    /* if game over then print score */
    if (game->status == GameOver) {
        mvwprintw(window,
                  (renderer->height >> 1) - 1, /* divide by 2 */
                  (renderer->width >> 1) - 10, /* divide by 2 */
                  "Game Over! Score: %d",
                  game->score);
    }

    /* refresh window to draw new renderings */
    wrefresh(window);
}

void renderer_end(Renderer *renderer) {
    werase(renderer->main_window);
    endwin();
}
