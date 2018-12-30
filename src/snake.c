#include "snake.h"
#include <stdlib.h>

void snake_init(Snake *snake, unsigned int max_length,
                unsigned int initial_length, Position initial_position,
                Direction initial_direction) {
    unsigned int idx;

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

Position snake_get_head_position(Snake *snake) { return snake->cells[0]; }

void snake_grow(Snake *snake) {
    const unsigned int current_length = snake->length;
    const unsigned int last_cell_idx = current_length - 1;
    const unsigned int new_cell_idx = current_length;

    const Position last_cell = snake->cells[last_cell_idx];
    const Position new_cell = position_previous(last_cell, snake->direction);

    snake->cells[new_cell_idx] = new_cell;
    snake->length = current_length + 1;
}

void snake_step(Snake *snake) {
    unsigned int idx;
    for (idx = snake->length - 1; idx > 0; idx--) {
        snake->cells[idx] = snake->cells[idx - 1];
    }
    snake->cells[0] = position_next(snake->cells[0], snake->direction);
}

void snake_turn(Snake *snake, Direction new_direction) {
    const Direction current_direction = snake->direction;
    if (/* UP or DOWN direction changes are allowed only on LEFT or RIGHT
           existing directions */
        ((new_direction == DirectionUp || new_direction == DirectionDown) &&
         (current_direction == DirectionLeft ||
          current_direction == DirectionRight)) ||
        /* LEFT or RIGHT direction changes are allowed only on UP or DOWN
           existing directions */
        ((new_direction == DirectionLeft || new_direction == DirectionRight) &&
         (current_direction == DirectionUp ||
          current_direction == DirectionDown))) {
        /* change to new direction */
        snake->direction = new_direction;
    }
}

static int is_snake_cell(Snake *snake, unsigned int from, Position position) {
    unsigned int idx;
    for (idx = from; idx < snake->length; idx++) {
        if (position_equal(snake->cells[idx], position)) {
            return 1;
        }
    }
    return 0;
}

/* check all cells of the snake's body */
int snake_is_body_cell_position(Snake *snake, Position position) {
    return is_snake_cell(snake, 0, position);
}

/* check all cells of the snake's body except the head */
int snake_is_tail_cell_position(Snake *snake, Position position) {
    return is_snake_cell(snake, 1, position);
}
