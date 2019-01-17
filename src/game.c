#include "game.h"
#include "position.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static const unsigned int FRAMES_PER_SECOND = 10;
static const float MILLISECONDS_PER_FRAME = 1000 / FRAMES_PER_SECOND;
static const unsigned int INPUT_QUEUE_INITIAL_CAPACITY = 32;

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

static void game_init(Game *game, unsigned int game_width, unsigned int game_height) {
    Position initial_snake_position;

    if (game->input_queue != NULL) {
        free(game->input_queue);
    }

    game->input_queue = (GameInput *)malloc(sizeof(GameInput) * INPUT_QUEUE_INITIAL_CAPACITY);
    ASSERT_ALLOC(game->input_queue);
    game->input_queue_capacity = INPUT_QUEUE_INITIAL_CAPACITY;
    game->input_queue_count = 0;

    initial_snake_position.x = (int)game_width / 2;
    initial_snake_position.y = (int)game_height / 2;
    snake_init(&game->snake, initial_snake_position, DirectionLeft);

    game->width = game_width;
    game->height = game_height;
    game->max_score = game_width * game_height - 1;
    game->score = 0;
    game->milliseconds_per_frame = MILLISECONDS_PER_FRAME;
    game->status = Playing;
    generate_random_food(game);
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

/*
static GameInput game_peek_input_queue(Game *game) {
    if (game->input_queue_count > 0) {
        return game->input_queue[0];
    }
    return NoInput;
}
*/

static GameInput game_pop_input_queue(Game *game) {
    const unsigned int current_queue_count = game->input_queue_count;
    GameInput input;

    if (current_queue_count > 0) {
        input = game->input_queue[0];

        /* remove element from queue */
        game->input_queue_count--;
        memmove(game->input_queue, game->input_queue + 1, sizeof(GameInput) * (current_queue_count - 1));

        return input;
    }

    return -1;
}

static void game_process_input(Game *game, GameInput input) {
    const GameStatus status = game->status;

    if (status == Playing) {
        switch (input) {
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
        case KeyNewGame:
            game_init(game, game->width, game->height);
            break;
        case KeyPause:
            game->status = Paused;
            break;
        case KeyQuit:
            game->status = GameOver;
            break;
        case KeySpeedIncrease:
            game->milliseconds_per_frame *= 0.9;
            break;
        case KeySpeedDecrease:
            game->milliseconds_per_frame *= 1.1;
            break;
        default:
            break;
        }
        return;
    }

    if (status == Paused) {
        switch (input) {
        case KeyNewGame:
            game_init(game, game->width, game->height);
            break;
        case KeyPause:
            game->status = Playing;
            break;
        case KeyQuit:
            game->status = GameOver;
            break;
        case KeySpeedIncrease:
            game->milliseconds_per_frame *= 0.9;
            break;
        case KeySpeedDecrease:
            game->milliseconds_per_frame *= 1.1;
            break;
        default:
            break;
        }
        return;
    }

    if (status == GameOver) {
        switch (input) {
        case KeyNewGame:
            game_init(game, game->width, game->height);
            break;
        case KeyQuit:
            game->status = Quit;
            break;
        default:
            break;
        }
        return;
    }
}

Game *game_new(unsigned int game_width, unsigned int game_height) {
    Game *game = (Game *)malloc(sizeof(Game));
    ASSERT_ALLOC(game);
    game_init(game, game_width, game_height);
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

void game_queue_input(Game *game, GameInput input) {
    unsigned int new_max_size;

    /* if exceeding max_length then reallocate current max capacity + INPUT_QUEUE_INITIAL_CAPACITY */
    if (game->input_queue_count >= game->input_queue_capacity) {
        new_max_size = game->input_queue_capacity + INPUT_QUEUE_INITIAL_CAPACITY;
        game->input_queue = realloc(game->input_queue, sizeof(GameInput) * new_max_size);
        ASSERT_ALLOC(game->input_queue);
        game->input_queue_capacity = new_max_size;
    }

    game->input_queue[game->input_queue_count] = input;
    game->input_queue_count++;
}

void game_update(Game *game) {
    GameInput input;
    Snake *snake;
    Position next_head_position;

    if (game->input_queue_count > 0) {
        input = game_pop_input_queue(game);
        game_process_input(game, input);
    }

    /* if not playing then do not process updates */
    if (game->status != Playing) {
        return;
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
            return;
        }

        generate_random_food(game);
        return;
    }

    /* step snake forward to next location */
    snake_step_forward(snake);
}
