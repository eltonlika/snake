#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int uint;

typedef enum { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 } Direction;

/* amount of change on plane X for each step, indexed by Direction */
static const signed char XDelta[4] = {0, 1, 0, -1};

/* amount of change on plane Y for each step, indexed by Direction */
static const signed char YDelta[4] = {-1, 0, 1, 0};

typedef struct {
    uint x;
    uint y;
} Position;

typedef struct {
    uint max_length;
    uint length;
    Direction direction;
    Position *cells;
} Snake;

typedef struct {
    uint width;
    uint height;
    Position food;
    Snake snake;
} Game;

static int random_number(int min_num, int max_num) {
    int low_num, hi_num;

    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num + 1; /* include max_num in output */
    } else {
        low_num = max_num + 1; /* include max_num in output */
        hi_num = min_num;
    }

    return (rand() % (hi_num - low_num)) + low_num;
}

static Position position_random(uint width, uint height) {
    Position random;
    random.x = (uint)random_number(0, width);
    random.y = (uint)random_number(0, height);
    return random;
}

static uint position_equal(Position p1, Position p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

static Position position_previous(Position position, Direction direction) {
    Position previous;
    previous.x = position.x - XDelta[direction];
    previous.y = position.y - YDelta[direction];
    return previous;
}

static Position position_next(Position position, Direction direction) {
    Position next;
    next.x = position.x + XDelta[direction];
    next.y = position.y + YDelta[direction];
    return next;
}

static void snake_init(Snake *snake, uint max_length, uint initial_length,
                       Position initial_position, Direction initial_direction) {
    uint idx;

    snake->max_length = max_length;
    snake->length = initial_length;
    snake->direction = initial_direction;
    snake->cells = (Position *)malloc(sizeof(Position) * max_length);
    snake->cells[0] = initial_position;

    for (idx = 1; idx < initial_length; idx++) {
        snake->cells[idx] =
            position_previous(snake->cells[idx - 1], initial_direction);
    }
}

static Position snake_ahead(Snake *snake) {
    return position_next(snake->cells[0], snake->direction);
}

static void snake_grow(Snake *snake) {
    const uint current_length = snake->length;
    const uint last_cell_idx = current_length - 1;
    const uint new_cell_idx = current_length;
    const Position last_cell = snake->cells[last_cell_idx];
    const Position new_cell = position_previous(last_cell, snake->direction);
    snake->cells[new_cell_idx] = new_cell;
    snake->length = current_length + 1;
}

static void snake_step(Snake *snake) {
    uint idx;
    for (idx = snake->length - 1; idx > 0; idx--) {
        snake->cells[idx] = snake->cells[idx - 1];
    }
    snake->cells[0] = snake_ahead(snake);
}

static void snake_turn(Snake *snake, Direction new_direction) {
    const Direction current_direction = snake->direction;
    if (/* UP or DOWN direction changes are allowed only on LEFT or RIGHT
           existing directions */
        ((new_direction == UP || new_direction == DOWN) &&
         (current_direction == LEFT || current_direction == RIGHT)) ||
        /* LEFT or RIGHT direction changes are allowed only on UP or DOWN
           existing directions */
        ((new_direction == LEFT || new_direction == RIGHT) &&
         (current_direction == UP || current_direction == DOWN))) {
        /* change to new direction */
        snake->direction = new_direction;
    }
}

static uint snake_is_body_cell(Snake *snake, Position position) {
    uint idx;
    for (idx = 0; idx < snake->length; idx++) {
        if (position_equal(snake->cells[idx], position)) {
            return 1;
        }
    }
    return 0;
}

static void snake_print(Snake *snake) {
    uint idx;
    for (idx = 0; idx < snake->length; idx++) {
        printf("%d %d\n", snake->cells[idx].x, snake->cells[idx].y);
    }
    printf("--------------\n");
}

Game *game_init() {
    uint game_width, game_height, max_snake_length, initial_snake_length;
    Position initial_snake_position, initial_food_position;
    Direction initial_snake_direction;
    Game *game;
    Snake *snake;

    /* initialize random generator */
    srand(time(NULL));

    game_width = 80;
    game_height = 80;
    max_snake_length = game_width * game_height;
    initial_snake_length = 3;
    initial_snake_position.x = game_width / 2;
    initial_snake_position.y = game_height / 2;
    initial_snake_direction = DOWN;

    game = malloc(sizeof(Game));
    game->width = game_width;
    game->height = game_height;

    snake = &game->snake;
    snake_init(snake, max_snake_length, initial_snake_length,
               initial_snake_position, initial_snake_direction);

    /* generate random food position which does not collide with snake body */
    do {
        initial_food_position = position_random(game_width, game_height);
    } while (snake_is_body_cell(snake, initial_food_position));

    game->food = initial_food_position;

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

int main() {
    Game *game = game_init();
    Snake *snake = &game->snake;
    snake_print(snake);

    snake_grow(snake);
    snake_print(snake);

    snake_step(snake);
    snake_print(snake);

    snake_turn(snake, RIGHT);
    snake_step(snake);
    snake_print(snake);

    snake_step(snake);
    snake_print(snake);

    game_free(game);

    return 0;
}
