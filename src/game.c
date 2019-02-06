#include "game.h"
#include "position.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static const float MILLISECONDS_PER_FRAME = 100;
static const unsigned int INPUT_QUEUE_INITIAL_CAPACITY = 32;

/* collision & endgame checks */
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
    return game->score >= game->max_score;
}

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
             position_equal(old_food_position, new_random_food_position));

    game->food = new_random_food_position;
}

static void game_init(Game *game, unsigned int game_width, unsigned int game_height) {
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
    game->max_score = (game_width * game_height) - 1;
    game->score = 0;
    game->milliseconds_per_frame = MILLISECONDS_PER_FRAME;
    game->status = Playing;
    generate_random_food(game);
}

static void game_input_queue_add(Game *game, GameInput input) {
    unsigned int input_queue_count = game->input_queue_count;

    /* do not queue new input if it is the same as the last input on queue */
    if (input_queue_count > 0 && game->input_queue[input_queue_count - 1] == input) {
        return;
    }

    /* if exceeding max_length then reallocate current max capacity + INPUT_QUEUE_INITIAL_CAPACITY */
    if (input_queue_count >= game->input_queue_capacity) {
        game->input_queue_capacity += INPUT_QUEUE_INITIAL_CAPACITY;
        game->input_queue = realloc(game->input_queue, sizeof(GameInput) * game->input_queue_capacity);
        ASSERT_ALLOC(game->input_queue);
    }

    /* queue snake control inputs to the back of the queue */
    game->input_queue[input_queue_count] = input;
    game->input_queue_count = input_queue_count + 1;
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

static void game_input_process_snake_control(Game *game, GameInput input) {
    if (game->status == Playing) {
        if (input == KeyUp) {
            snake_turn(&game->snake, DirectionUp);
        }

        else if (input == KeyRight) {
            snake_turn(&game->snake, DirectionRight);
        }

        else if (input == KeyDown) {
            snake_turn(&game->snake, DirectionDown);
        }

        else if (input == KeyLeft) {
            snake_turn(&game->snake, DirectionLeft);
        }
    }
}

static void game_input_process_menu_control(Game *game, GameInput input) {
    if (input == KeyNewGame) {
        game_init(game, game->width, game->height);
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
            game->milliseconds_per_frame *= 0.9;
        }
    }

    else if (input == KeySpeedDecrease) {
        if (game->status == Playing) {
            game->milliseconds_per_frame *= 1.1;
        }
    }
}

Game *game_new(unsigned int game_width, unsigned int game_height) {
    Game *game = (Game *)malloc(sizeof(Game));
    ASSERT_ALLOC(game);
    game_init(game, game_width, game_height);
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
    GameInput input;
    Snake *snake;
    Position next_head_position;

    /* if not playing then do not process updates */
    if (game->status != Playing) {
        return;
    }

    if (game->input_queue_count > 0) {
        input = game_input_queue_pop(game);
        game_input_process_snake_control(game, input);
    }

    snake = &game->snake;
    next_head_position = snake_get_next_head_position(snake);

    /* check if snake will collide with itself */
    if (collides_snake(game, next_head_position)) {
        game->status = GameOver;
        return;
    }

    /* check if snake will collide with the borders/walls */
    if (collides_border(game, next_head_position)) {
        game->status = GameOver;
        return;
    }

    /* check if snake's head will eat the food */
    if (collides_food(game, next_head_position)) {
        snake_step_forward(snake);
        snake_grow(snake);
        game->score++;

        /* if snake reached maximum score then game over */
        if (max_score_reached(game)) {
            game->status = GameOver;
        } else {
            generate_random_food(game);
        }

        return;
    }

    /* step snake forward to next location */
    snake_step_forward(snake);
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
