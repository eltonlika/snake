#ifndef SNAKE_H
#define SNAKE_H

#include "position.h"

typedef struct {
    unsigned int max_length;
    unsigned int length;
    Direction direction;
    Position *cells;
} Snake;

void snake_init(Snake *snake, unsigned int max_length,
                unsigned int initial_length, Position initial_position,
                Direction initial_direction);

Position snake_get_head_position(Snake *snake);

int snake_is_body_cell_position(Snake *snake, Position position);

int snake_is_tail_cell_position(Snake *snake, Position position);

void snake_step(Snake *snake);

void snake_turn(Snake *snake, Direction new_direction);

void snake_grow(Snake *snake);

void snake_print(Snake *snake);

#endif /* SNAKE_H */
