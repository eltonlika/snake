#include "renderer.h"
#include "utils.h"
#include <curses.h>
#include <stdlib.h>

#define FOOD_PAIR 1
#define SNAKE_PAIR 2
#define WALL_PAIR 3

Renderer *renderer_init() {
    Renderer *renderer = malloc(sizeof(Renderer));
    ASSERT_ALLOC(renderer);

    /* create main window */
    renderer->window = initscr();
    curs_set(0); /* set cursor invisible */

    /* if supports colors then initialize them */
    if (has_colors() == TRUE) {
        start_color();
        init_pair(FOOD_PAIR, COLOR_RED, COLOR_BLACK);
        init_pair(SNAKE_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(WALL_PAIR, COLOR_BLUE, COLOR_BLACK);
        renderer->color = True;
    } else {
        renderer->color = False;
    }

    /* set playfield width and height smaller to account for main window borders */
    getmaxyx(renderer->window, renderer->height, renderer->width);

    werase(renderer->window);
    wrefresh(renderer->window);

    return renderer;
}

void renderer_free(Renderer *renderer) {
    if (renderer != NULL) {
        free(renderer);
    }
}

static const char food_character = 'x';
static const char body_character = 'o';
static const char head_characters[4] = {'^', '>', 'v', '<'};

void renderer_render(Renderer *renderer, Game *game) {
    const Snake *snake = &game->snake;
    const unsigned int snake_length = snake->length;
    const Position *snake_cells = snake->cells;
    const Position snake_head = snake_cells[0];
    const Position food_pos = game->food;
    const char head_character = head_characters[snake->direction];
    const Bool color = renderer->color;
    WINDOW *window = renderer->window;
    Position cell_pos;
    unsigned int idx;

    werase(window);

    /* if game has walls then render them at screen border */
    if (game->walls) {
        if (color) {
            attron(COLOR_PAIR(WALL_PAIR));
        }
        box(window, 0, 0);
        if (color) {
            attroff(COLOR_PAIR(WALL_PAIR));
        }
    }

    /* render score and frames per second */
    mvwprintw(window,
              0,
              renderer->width - 30,
              " Score: %d  -  fps: %d ",
              game->score,
              1000 / game->milliseconds_per_frame);

    /* render food */
    if (color) {
        attron(COLOR_PAIR(FOOD_PAIR));
    }
    mvwaddch(window, food_pos.y, food_pos.x, food_character);
    if (color) {
        attroff(COLOR_PAIR(FOOD_PAIR));
    }

    /* render snake */
    if (color) {
        attron(COLOR_PAIR(SNAKE_PAIR));
    }

    /* render snake tail */
    for (idx = 1; idx < snake_length; idx++) {
        cell_pos = snake_cells[idx];
        mvwaddch(window, cell_pos.y, cell_pos.x, body_character);
    }

    /* render snake head */
    mvwaddch(window, snake_head.y, snake_head.x, head_character);

    if (color) {
        attroff(COLOR_PAIR(SNAKE_PAIR));
    }

    /* if game over then print score */
    if (game->status == GameOver) {
        mvwprintw(window,
                  (renderer->height >> 1) - 1, /* divide by 2 */
                  (renderer->width >> 1) - 11, /* divide by 2 */
                  "Game Over! Score: %d",
                  game->score);
    } else if (game->status == Paused) {
        mvwprintw(window,
                  (renderer->height >> 1) - 1, /* divide by 2 */
                  (renderer->width >> 1) - 34, /* divide by 2 */
                  "Paused. Press 'p' to resume game, 'n' for new game or 'q' to quit.",
                  game->score);
    }

    /* refresh window to draw new renderings */
    wrefresh(window);
}

void renderer_end(Renderer *renderer) {
    werase(renderer->window);
    delwin(renderer->window);
    endwin();
}
