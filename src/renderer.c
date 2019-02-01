#include "renderer.h"
#include "utils.h"
#include <curses.h>
#include <stdlib.h>

Renderer *renderer_init() {
    unsigned int screen_width, screen_height;

    Renderer *renderer = malloc(sizeof(Renderer));
    ASSERT_ALLOC(renderer);

    /* create main window */
    renderer->main_window = initscr();
    curs_set(0); /* set cursor invisible */

    /* set playfield width and height smaller to account for main window borders */
    getmaxyx(renderer->main_window, screen_height, screen_width);
    renderer->playfield_width = screen_width - 2;
    renderer->playfield_height = screen_height - 2;

    /* create playfield window */
    renderer->playfield_window = derwin(renderer->main_window,
                                        renderer->playfield_height, renderer->playfield_width,
                                        1, 1);

    werase(renderer->main_window);
    box(renderer->main_window, 0, 0); /* render main window border */
    wrefresh(renderer->main_window);

    return renderer;
}

void renderer_free(Renderer *renderer) {
    if (renderer != NULL) {
        free(renderer);
    }
}

static const char food_character = 'X';
static const char body_character = '#';
static const char head_characters[4] = {'A', '>', 'V', '<'};

void renderer_render(Renderer *renderer, Game *game) {
    const Snake *snake = &game->snake;
    const Position *cells = snake->cells;
    const Position snake_head = cells[0];
    const Position food_pos = game->food;
    const unsigned int snake_length = snake->length;
    const char head_character = head_characters[snake->direction];
    WINDOW *playfield_window = renderer->playfield_window;
    Position cell_pos;
    unsigned int idx;

    werase(playfield_window);

    /* render tail */
    for (idx = 1; idx < snake_length; idx++) {
        cell_pos = cells[idx];
        mvwaddch(playfield_window, cell_pos.y, cell_pos.x, body_character);
    }

    /* render snake head */
    mvwaddch(playfield_window, snake_head.y, snake_head.x, head_character);

    /* render food */
    mvwaddch(playfield_window, food_pos.y, food_pos.x, food_character);

    /* if game over then print score */
    if (game->status == GameOver) {
        mvwprintw(playfield_window,
                  (renderer->playfield_height >> 1) - 1, /* divide by 2 */
                  (renderer->playfield_width >> 1) - 10, /* divide by 2 */
                  "Game Over! Score: %d",
                  game->score);
    }

    /* refresh playground window to draw new renderings */
    wrefresh(playfield_window);
}

void renderer_end(Renderer *renderer) {
    werase(renderer->main_window);
    delwin(renderer->playfield_window);
    delwin(renderer->main_window);
    endwin();
}
