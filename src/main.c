#include "game.h"
#include "input.h"
#include "renderer.h"
#include "utils.h"
#include <stdio.h>

int main() {
    Renderer *renderer;
    Game *game;
    GameInput input_key;

    /* initialize random number generator */
    random_init();

    /* initialize rendering window */
    renderer = renderer_init();

    /* initialize curses input */
    input_init(renderer->main_window);

    /* initialize game structure */
    game = game_new(renderer->width, renderer->height);

    /* render first frame of new game */
    renderer_render(renderer, game);

    /* main game loop */
    while (game->status != Quit) {

        /* add all buffered input keys to game input queue */
        while ((input_key = input_get_next_key(renderer->main_window)) != NoInput) {
            game_queue_input(game, input_key);
        }

        /* update game */
        game_update(game);

        /* render game */
        renderer_render(renderer, game);

        /* sleep */
        timer_sleep(game->milliseconds_per_frame);
    }

    /* stop renderer */
    renderer_end(renderer);

    /* free memory */
    renderer_free(renderer);
    game_free(game);

    return 0;
}
