#ifndef SNAKE_H
#define SNAKE_H

#include "position.h"
#include "utils.h"

typedef struct {
    unsigned int max_length;
    unsigned int length;
    Direction direction;
    Position *cells;
} Snake;

void snake_init(Snake *snake, Position initial_position, Direction initial_direction);

void snake_free(Snake *snake);

Bool snake_occupies_position(Snake *snake, Position position);

void snake_grow(Snake *snake);

void snake_step_forward(Snake *snake, Position new_head_cell);

void snake_turn(Snake *snake, Direction new_direction);

#endif /* SNAKE_H */
