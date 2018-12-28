#include "snake.h"
#include <stdio.h>

void snake_init(Snake *snake, uint max_length, uint initial_length,
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

Position snake_get_position_ahead(Snake *snake) {
    return position_next(snake->cells[0], snake->direction);
}

void snake_grow(Snake *snake) {
    const uint current_length = snake->length;
    const uint last_cell_idx = current_length - 1;
    const uint new_cell_idx = current_length;

    const Position last_cell = snake->cells[last_cell_idx];
    const Position new_cell = position_previous(last_cell, snake->direction);

    snake->cells[new_cell_idx] = new_cell;
    snake->length = current_length + 1;
}

void snake_step(Snake *snake) {
    uint idx;
    for (idx = snake->length - 1; idx > 0; idx--) {
        snake->cells[idx] = snake->cells[idx - 1];
    }
    snake->cells[0] = snake_get_position_ahead(snake);
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

uint snake_is_body_cell(Snake *snake, Position position) {
    uint idx;
    for (idx = 0; idx < snake->length; idx++) {
        if (position_equal(snake->cells[idx], position)) {
            return 1;
        }
    }
    return 0;
}

void snake_print(Snake *snake) {
    uint idx;
    for (idx = 0; idx < snake->length; idx++) {
        printf("%d %d\n", snake->cells[idx].x, snake->cells[idx].y);
    }
    printf("--------------\n");
}
