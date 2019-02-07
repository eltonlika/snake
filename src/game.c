#include "game.h"
#include "position.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static const float MILLISECONDS_PER_FRAME = 100;
static const unsigned int INPUT_QUEUE_INITIAL_CAPACITY = 32;

static Bool position_outside_limits(Position position, int game_width, int game_height){
    return (position.x < 0 || position.x >= game_width ||
            position.y < 0 || position.y >= game_height);
}

static Position position_wrap_around(Position position, int game_width, int game_height){
    if(position.x < 0) {
        position.x += game_width;
    }else if(position.x >= game_width) {
        position.x -= game_width;
    }

    if(position.y < 0) {
        position.y += game_height;
    }else if(position.y >= game_height) {
        position.y -= game_height;
    }

    return position;
}

static Bool collides_walls(Game *game, Position check_position) {
    return  game->walls &&
            (check_position.x <= 0 || check_position.x >= ((int)game->width - 1) ||
             check_position.y <= 0 || check_position.y >= ((int)game->height - 1));
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

static void game_set_walls(Game *game, Bool enable_walls){
    if(!game->walls && enable_walls){
        game->score_multiplier = 2;
        game->walls = enable_walls;

        if(position_outside_limits(game->food, game->width, game->height)){
            generate_random_food(game);
        }
    }
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

    game->walls = walls;
    game->width = game_width;
    game->height = game_height;
    game->score_multiplier = walls ? 2 : 1; 
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
            game->milliseconds_per_frame *= 0.9;
        }
    }

    else if (input == KeySpeedDecrease) {
        if (game->status == Playing) {
            game->milliseconds_per_frame *= 1.1;
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

    /* check if snake will collide with the walls */
    if (collides_walls(game, next_head_position)) {
        game->status = GameOver;
        return;
    }

    /* check if snake's head will eat the food */
    if (collides_food(game, next_head_position)) {
        snake_step_forward(snake);
        snake_grow(snake);
        game->score++;
        generate_random_food(game);
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
