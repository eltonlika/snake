#include "game.h"
#include "position.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

/* speed level values are milliseconds of wait time per game loop iteration */
static const unsigned int SPEED_LEVELS[] = {400, 200, 133, 100, 80, 66, 57, 50, 25, 12, 6};
const char *SPEED_LEVELS_DESCR[] = {"0.25x", "0.5x", "0.75x", "1x", "1.25x", "1.5x", "1.75x", "2x", "4x", "8x", "16x"};
static const unsigned int NUM_OF_SPEED_LEVELS = sizeof(SPEED_LEVELS) / sizeof(SPEED_LEVELS[0]);
static const unsigned int DEFAULT_SPEED_LEVEL = 3; /* default speed level is index of 1x (100ms) */

static const unsigned int INPUT_QUEUE_INITIAL_CAPACITY = 32;

static Bool position_outside_limits(Position position, int game_width, int game_height) {
    return (position.x < 0 || position.x >= game_width ||
            position.y < 0 || position.y >= game_height);
}

static Position position_wrap_around(Position position, int game_width, int game_height) {
    if (position.x < 0) {
        position.x += game_width;
    } else if (position.x >= game_width) {
        position.x -= game_width;
    }

    if (position.y < 0) {
        position.y += game_height;
    } else if (position.y >= game_height) {
        position.y -= game_height;
    }

    return position;
}

static Bool collides_walls(Game *game, Position check_position) {
    return (check_position.x == 0 || check_position.x == ((int)game->width - 1) ||
            check_position.y == 0 || check_position.y == ((int)game->height - 1));
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

static Bool collides_food(Game *game, Position check_position) {
    return position_equal(game->food, check_position);
}

static Bool max_snake_length_reached(Game *game) {
    return game->snake.length >= game->max_snake_length;
}

/* generate random food position which does not collide with snake body or walls */
static void generate_random_food(Game *game) {
    const Position old_food_position = game->food;
    Snake *snake = &game->snake;
    int min_x, min_y, max_x, max_y;
    Position new_random_food_position;

    if (game->walls) {
        min_x = 1;
        min_y = 1;
        max_x = game->width - 2;
        max_y = game->height - 2;
    } else {
        min_x = 0;
        min_y = 0;
        max_x = game->width - 1;
        max_y = game->height - 1;
    }

    do {
        new_random_food_position = position_random(min_x, min_y, max_x, max_y);
    } while (snake_occupies_position(snake, new_random_food_position) ||
             position_equal(old_food_position, new_random_food_position));

    game->food = new_random_food_position;
}

static void game_speed_reset(Game *game) {
    game->speed_level = DEFAULT_SPEED_LEVEL;
    game->milliseconds_per_frame = SPEED_LEVELS[game->speed_level];
}

static void game_speed_increase(Game *game) {
    if (game->speed_level < (NUM_OF_SPEED_LEVELS - 1)) {
        game->speed_level++;
        game->milliseconds_per_frame = SPEED_LEVELS[game->speed_level];
    }
}

static void game_speed_decrease(Game *game) {
    if (game->speed_level > 0) {
        game->speed_level--;
        game->milliseconds_per_frame = SPEED_LEVELS[game->speed_level];
    }
}

static Bool game_toggle_walls(Game *game) {
    unsigned int idx;

    if (game->walls) {
        game->walls = False;
        game->max_snake_length = game->width * game->height;
    } else {
        game->walls = True;
        game->max_snake_length = (game->width - 2) * (game->height - 2);

        /* if snake is over walls then game over */
        for (idx = 0; idx < game->snake.length; idx++) {
            if (collides_walls(game, game->snake.cells[idx])) {
                game->status = GameOver;
                return True;
            }
        }

        /* if food is over walls then move food to new random location */
        if (collides_walls(game, game->food)) {
            generate_random_food(game);
        }
    }

    return game->walls;
}

static void game_init(Game *game, unsigned int game_width, unsigned int game_height, Bool walls) {
    Position initial_snake_position;

    if (game->input_queue != NULL) {
        free(game->input_queue);
    }

    game->input_queue = (GameInput *)malloc(sizeof(GameInput) * INPUT_QUEUE_INITIAL_CAPACITY);
    ASSERT_ALLOC(game->input_queue);
    game->input_queue_capacity = INPUT_QUEUE_INITIAL_CAPACITY;
    game->input_queue_count = 0;

    /* snake is initially on center of screen */
    initial_snake_position.x = (int)game_width >> 1;
    initial_snake_position.y = (int)game_height >> 1;
    snake_init(&game->snake, initial_snake_position, DirectionLeft);

    game->width = game_width;
    game->height = game_height;
    game->max_snake_length = walls ? ((game_width - 2) * (game_height - 2)) : (game_width * game_height);
    game->score = 0;
    game->speed_level = DEFAULT_SPEED_LEVEL;
    game->milliseconds_per_frame = SPEED_LEVELS[DEFAULT_SPEED_LEVEL];
    game->walls = walls;
    game->status = Playing;

    generate_random_food(game);
}

static void game_input_queue_add(Game *game, GameInput input) {
    /* do not queue new input if it is the same as the last input on queue */
    if (game->input_queue_count > 0 &&
        game->input_queue[game->input_queue_count - 1] == input) {
        return;
    }

    /* if exceeding max_length then reallocate current max capacity + INPUT_QUEUE_INITIAL_CAPACITY */
    if (game->input_queue_count >= game->input_queue_capacity) {
        game->input_queue_capacity += INPUT_QUEUE_INITIAL_CAPACITY;
        game->input_queue = realloc(game->input_queue, sizeof(GameInput) * game->input_queue_capacity);
        ASSERT_ALLOC(game->input_queue);
    }

    /* queue snake control inputs to the back of the queue */
    game->input_queue[game->input_queue_count] = input;
    game->input_queue_count++;
}

static GameInput game_input_queue_pop(Game *game) {
    GameInput input;

    if (game->input_queue_count > 0) {
        input = game->input_queue[0];
        /* remove element from queue */
        game->input_queue_count--;
        memmove(game->input_queue, game->input_queue + 1, sizeof(GameInput) * game->input_queue_count);
        return input;
    }

    return NoInput;
}

static void game_input_process_menu_control(Game *game, GameInput input) {
    if (input == KeyNewGame) {
        game_init(game, game->width, game->height, game->walls);
    }

    else if (input == KeyPause) {
        if (game->status == Playing) {
            game->status = Paused;
        } else if (game->status == Paused) {
            game->status = Playing;
        }
    }

    else if (input == KeyQuit) {
        if (game->status == GameOver) {
            game->status = Quit;
        } else {
            game->status = GameOver;
        }
    }

    else if (input == KeySpeedIncrease) {
        if (game->status == Playing) {
            game_speed_increase(game);
        }
    }

    else if (input == KeySpeedDecrease) {
        if (game->status == Playing) {
            game_speed_decrease(game);
        }
    }

    else if (input == KeySpeedReset) {
        if (game->status == Playing) {
            game_speed_reset(game);
        }
    }

    else if (input == KeyToggleWalls) {
        if (game->status == Playing) {
            game_toggle_walls(game);
        }
    }
}

Game *game_new(unsigned int game_width, unsigned int game_height, Bool walls) {
    Game *game = (Game *)malloc(sizeof(Game));
    ASSERT_ALLOC(game);
    game_init(game, game_width, game_height, walls);
    return game;
}

void game_input(Game *game, GameInput input) {
    if (input == NoInput) {
        return;
    }

    if (input_is_snake_control(input)) {
        if (game->status == Playing) {
            game_input_queue_add(game, input);
        }
    } else {
        game_input_process_menu_control(game, input);
    }
}

void game_update(Game *game) {
    const GameInput input = game_input_queue_pop(game);
    Snake *snake = &game->snake;
    Position next_head_position;

    /* check if has inputs to change snake direction */
    if (input != NoInput) {
        if (input == KeyUp) {
            snake_turn(snake, DirectionUp);
        } else if (input == KeyRight) {
            snake_turn(snake, DirectionRight);
        } else if (input == KeyDown) {
            snake_turn(snake, DirectionDown);
        } else if (input == KeyLeft) {
            snake_turn(snake, DirectionLeft);
        }
    }

    /* calculate next head/forward position */
    next_head_position = position_next(snake->cells[0], snake->direction);

    if (game->walls) {
        /* if game has walls then check if new snake head will collide with the walls */
        if (collides_walls(game, next_head_position)) {
            game->status = GameOver;
            return;
        }
    } else {
        /* if game has no walls then check if new snake head position
            has overflown and needs to be wrapped around */
        if (position_outside_limits(next_head_position, game->width, game->height)) {
            next_head_position = position_wrap_around(next_head_position, game->width, game->height);
        }
    }

    /* check if snake will collide with itself */
    if (collides_snake(game, next_head_position)) {
        game->status = GameOver;
        return;
    }

    /* check if new snake head will eat the food */
    if (collides_food(game, next_head_position)) {
        /* Note: there is no need to calculate and set the new tail cell position
                 because it is going to be overwritten in the snake_step_forward call below
                 with the second-to-last cell position value */
        snake_grow(snake); /* increases snake->length and allocates memory for new cells */
        game->score++;

        if (max_snake_length_reached(game)) {
            game->status = GameOver;
        } else {
            generate_random_food(game);
        }
    }

    /* step snake forward to next position */
    snake_step_forward(snake, next_head_position);
}

void game_free(Game *game) {
    if (game != NULL) {
        snake_free(&game->snake);
        if (game->input_queue != NULL) {
            free(game->input_queue);
        }
        free(game);
    }
}
