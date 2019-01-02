#include "game.h"
#include "position.h"
#include "utils.h"
#include <stdlib.h>

const unsigned int FRAMES_PER_SECOND = 10;
const float MICROSECONDS_PER_FRAME = 1000000 / FRAMES_PER_SECOND;

/* generate random food position which does not collide with snake body */
static void generate_random_food(Game *game) {
    const unsigned int max_x = game->width - 1;
    const unsigned int max_y = game->height - 1;
    const Position old_food_position = game->food;
    Snake *snake = &game->snake;
    Position new_random_food_position;

    do {
        new_random_food_position = position_random(max_x, max_y);
    } while (snake_occupies_position(snake, new_random_food_position) ||
             position_equal(new_random_food_position, old_food_position));

    game->food = new_random_food_position;
}

Game *game_new(unsigned int game_width, unsigned int game_height) {
    Game *game = malloc(sizeof(Game));
    ASSERT_ALLOC(game);

    game_init(game, game_width, game_height);
    return game;
}

void game_init(Game *game, unsigned int game_width, unsigned int game_height) {
    Position initial_snake_position;
    initial_snake_position.x = (int)game_width / 2;
    initial_snake_position.y = (int)game_height / 2;
    game->width = game_width;
    game->height = game_height;
    game->max_score = game_width * game_height - 1;
    game->score = 0;
    game->micros_per_frame = MICROSECONDS_PER_FRAME;
    game->status = Playing;
    snake_init(&game->snake, initial_snake_position, DirectionLeft);
    generate_random_food(game);
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

static Bool collides_snake(Game *game, Position check_position) {
    Snake *snake = &game->snake;
    const Position last_cell_position = snake->cells[snake->length - 1];
    if (position_equal(check_position, last_cell_position)) {
        /* no self collision because the current last cell will move */
        return False;
    }
    return snake_occupies_position(snake, check_position);
}

static Bool collides_border(Game *game, Position check_position) {
    const int newx = check_position.x;
    const int newy = check_position.y;
    return (newx < 0 || newx >= (int)game->width ||
            newy < 0 || newy >= (int)game->height);
}

static Bool collides_food(Game *game, Position check_position) {
    return position_equal(game->food, check_position);
}

static Bool max_score_reached(Game *game) {
    return game->score == game->max_score;
}

void game_update(Game *game) {
    Snake *snake;
    Position next_head_position;

    /* if not playing then do not process updates */
    if (game->status != Playing) {
        return;
    }

    snake = &game->snake;
    next_head_position = snake_get_next_head_position(snake);

    /* check if snake will collide with itself */
    if (collides_snake(game, next_head_position)) {
        game->status = Lost;
        return;
    }

    /* check if snake will collide with the borders/walls */
    if (collides_border(game, next_head_position)) {
        game->status = Lost;
        return;
    }

    /* check if snake's head will eat the food */
    if (collides_food(game, next_head_position)) {
        snake_step_forward(snake);
        snake_grow(snake);
        game->score++;

        /* if snake reached maximum score then game won */
        if (max_score_reached(game)) {
            game->status = Won;
            return;
        }

        generate_random_food(game);
        return;
    }

    /* step snake forward to next location */
    snake_step_forward(snake);
}

void game_process_input(Game *game, GameInput input) {
    /* if not playing then do not process input, unless it's the Pause key
     * (pressed again to unpause the game) */
    const int game_currently_paused = game->status == Paused;
    if (game_currently_paused && input != KeyPause) {
        return;
    }

    switch (input) {
    case NoInput:
        break;
    case KeyPause:
        game->status = (game_currently_paused ? Playing : Paused);
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
    case KeySpeedIncrease:
        game->micros_per_frame *= 0.9;
        break;
    case KeySpeedDecrease:
        game->micros_per_frame *= 1.1;
        break;
    default:
        break;
    }
}
