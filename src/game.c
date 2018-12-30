#include "game.h"
#include <stdlib.h>

/* generate random food position which does not collide with snake body */
static void game_generate_random_food(Game *game) {
    const unsigned int width = game->width;
    const unsigned int height = game->height;
    const Position old_food_position = game->food;
    Snake *snake = &game->snake;
    Position new_random_food_position;

    do {
        new_random_food_position = position_random(width, height);
    } while (snake_is_body_cell_position(snake, new_random_food_position) ||
             position_equal(old_food_position, new_random_food_position));

    game->food = new_random_food_position;
}

Game *game_init(unsigned int game_width, unsigned int game_height) {
    unsigned int initial_speed;
    unsigned int max_snake_length, initial_snake_length;
    Position initial_snake_position;
    Direction initial_snake_direction;
    Game *game;

    /* initial game parameters */
    initial_speed = 5; /* initial speed of the game (cells/second) */
    max_snake_length = game_width * game_height;
    initial_snake_length = 4;
    initial_snake_position.x = game_width / 2;
    initial_snake_position.y = game_height / 2;
    initial_snake_direction = DirectionDown;

    game = malloc(sizeof(Game));
    game->score = 0;
    game->width = game_width;
    game->height = game_height;
    game->speed = initial_speed;
    game->status = Playing;

    snake_init(&game->snake, max_snake_length, initial_snake_length,
               initial_snake_position, initial_snake_direction);

    game_generate_random_food(game);

    return game;
}

void game_free(Game *game) {
    if (game) {
        Position *cellsPtr = game->snake.cells;
        if (cellsPtr) {
            free(cellsPtr);
        }
        free(game);
    }
}

void game_update(Game *game) {
    Snake *snake;
    Position snake_head;

    /* if not playing then do not process updates */
    if (game->status != Playing) {
        return;
    }

    snake = &game->snake;

    /* step snake forward */
    snake_step(snake);

    snake_head = snake_get_head_position(snake);

    /* check if snake collided with it's tail */
    if (snake_is_tail_cell_position(snake, snake_head)) {
        game->status = Lost;
        return;
    }

    /* check if snake ate food */
    if (position_equal(game->food, snake_head)) {
        snake_grow(snake);
        game->score++;

        /* if snake reached maximum length then game won */
        if (snake->length == snake->max_length) {
            game->status = Won;
            return;
        }

        game_generate_random_food(game);
    }
}

void game_process_input(Game *game, GameInput input) {
    /* if not playing then do not process input, unless it's the Pause key
     * (pressed again to unpause the game) */
    if (game->status == Paused && input != KeyPause) {
        return;
    }

    switch (input) {
    case NoInput:
        break;
    case KeyPause:
        game->status = (game->status == Paused ? Playing : Paused);
        break;
    case KeyUp:
        snake_turn(&game->snake, DirectionUp);
        break;
    case KeyRight:
        snake_turn(&game->snake, DirectionRight);
        break;
    case KeyDown:
        snake_turn(&game->snake, DirectionDown);
        break;
    case KeyLeft:
        snake_turn(&game->snake, DirectionLeft);
        break;
    default:
        break;
    }
}
