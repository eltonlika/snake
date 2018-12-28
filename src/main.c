#include "game.h"
#include <stdio.h>

int main() {
    Game *game = game_init();
    Snake *snake = &game->snake;

    printf("Initial food position: %d %d\n", game->food.x, game->food.y);

    game_update(game);
    snake_print(snake);

    snake_turn(snake, DirectionRight);

    game_update(game);
    snake_print(snake);

    snake_turn(snake, DirectionUp);

    game_update(game);
    snake_print(snake);

    snake_turn(snake, DirectionLeft);

    game_update(game);
    snake_print(snake);

    game_update(game);
    snake_print(snake);

    game_update(game);
    snake_print(snake);

    game_free(game);

    return 0;
}
