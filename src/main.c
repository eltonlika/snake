#include "game.h"
#include "input.h"
#include "random.h"
#include "renderer.h"
#include "utils.h"
#include <stdio.h>

void quit(Screen *screen, Game *game);

int main() {
    Screen *screen;
    Game *game;
    GameInput input_key;
    GameStatus status;

    /* initialize random number generator */
    random_init();

    /* initialize rendering window */
    screen = renderer_init();

    /* initialize curses input */
    input_init(screen->main_window);

    /* initialize game */
    game = game_new(screen->width, screen->height);

    /* render first frame of new game */
    renderer_render(screen, game);

    /* main game loop */
    while (1) {
        /* if game ended then quit */
        status = game->status;
        if (status == Won || status == Lost) {
            quit(screen, game);
            return 0;
        }

        /* process game input (only last buffered key) */
        input_key = input_get_last_key(screen->main_window);
        if (input_key == NoInput) {
            /* left empty on purpose */
        } else if (input_key == KeyNewGame) {
            /* reinitialize game struct to create new game */
            game_init(game, screen->width, screen->height);
            /* render first frame of new game */
            renderer_render(screen, game);
            /* skip to next iteration */
            continue;
        } else if (input_key == KeyQuit) {
            quit(screen, game);
            return 0;
        } else {
            /* for every other key input, send it for game processing */
            game_process_input(game, input_key);
        }

        /* update game */
        game_update(game);

        /* render game */
        renderer_render(screen, game);

        game_sleep(game->milliseconds_per_frame);
    }

    return 0;
}

void quit(Screen *screen, Game *game) {
    /* stop renderer */
    renderer_end(screen);

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
