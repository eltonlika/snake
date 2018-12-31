#ifndef SNAKE_H
#define SNAKE_H

#include "position.h"

typedef struct {
    unsigned int max_length;
    unsigned int length;
    Direction direction;
    Position *cells;
} Snake;

void snake_init(Snake *snake, Position initial_position, Direction initial_direction);

Position snake_get_next_head_position(Snake *snake);

int snake_occupies_position(Snake *snake, Position position);

void snake_step_forward(Snake *snake);

void snake_grow(Snake *snake);

void snake_turn(Snake *snake, Direction new_direction);

#endif /* SNAKE_H */
