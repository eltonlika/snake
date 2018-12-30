#include "game.h"
#include "input.h"
#include "random.h"
#include "renderer.h"
#include <curses.h>
#include <stdio.h>

void quit(WINDOW *window, Game *game);

int main() {
    const unsigned int FRAMES_PER_SECOND = 10;
    const float MICROSECONDS_PER_FRAME = 1000000 / FRAMES_PER_SECOND;
    const unsigned int sleep_time_us = MICROSECONDS_PER_FRAME;

    unsigned int game_width, game_height;
    WINDOW *window;
    Game *game;
    GameInput input;
    GameStatus status;

    /* initialize random number generator */
    random_init();

    /* initialize rendering window */
    window = renderer_init();

    /* get console width and height */
    game_width = renderer_get_max_width(window);
    game_height = renderer_get_max_height(window);

    /* initialize game */
    game = game_init(game_width, game_height);

    /* render first frame of game state */
    renderer_render(window, game);

    /* main game loop */
    while (1) {

        /* if game ended then quit */
        status = game->status;
        if (status == Won || status == Lost) {
            quit(window, game);
            return 0;
        }

        /* process game input */
        input = input_get_key(window);
        if (input == NoInput) {
            /* left empty on purpose */
        } else if (input == KeyQuit) {
            quit(window, game);
            return 0;
        } else {
            game_process_input(game, input);
        }

        /* update game */
        game_update(game);

        /* render game */
        renderer_render(window, game);

        usleep(sleep_time_us);
    }

    return 0;
}

void quit(WINDOW *window, Game *game) {
    /* stop renderer */
    renderer_end(window);

    /* print game finish status */
    switch (game->status) {
    case Won:
        printf("You WON!!!\n\n");
        break;
    case Lost:
        printf("You lost.\n\n");
        break;
    default:
        break;
    }

    /* free memory */
    game_free(game);
}
