#include "game.h"
#include "utils.h"
#include <stdio.h>

/* generate random food position which does not collide with snake body */
static void game_generate_random_food(Game *game) {
    const uint width = game->width;
    const uint height = game->height;
    const Position old_food_position = game->food;
    Snake *snake = &game->snake;
    Position new_random_food_position;

    do {
        new_random_food_position = position_random(width, height);
    } while (snake_is_body_cell(snake, new_random_food_position) ||
             position_equal(old_food_position, new_random_food_position));

    game->food = new_random_food_position;
}

Game *game_init(uint game_width, uint game_height) {
    uint initial_speed;
    uint max_snake_length, initial_snake_length;
    Position initial_snake_position;
    Direction initial_snake_direction;
    Game *game;

    /* initialize random generator */
    init_random_generator();

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
    Position position_ahead;

    /* if not playing then do not process updates */
    if (game->status != Playing) {
        return;
    }

    snake = &game->snake;
    position_ahead = snake_get_position_ahead(snake);

    /* check if snake is about to collide with it's body */
    if (snake_is_body_cell(snake, position_ahead)) {
        game->status = Lost;
        /* printf("You lost! Snake ate itself!\n"); */
        return;
    }

    snake_step(snake);

    if (position_equal(game->food, position_ahead)) {
        snake_grow(snake);
        game->score++;
        /* printf("Snake ate one apple!\n"); */

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
        return;
    case KeyPause:
        game->status = Paused;
        break;
    case KeyUp:
        game->snake.direction = DirectionUp;
        break;
    case KeyRight:
        game->snake.direction = DirectionRight;
        break;
    case KeyDown:
        game->snake.direction = DirectionDown;
        break;
    case KeyLeft:
        game->snake.direction = DirectionLeft;
        break;
    default:
        break;
    }
}
