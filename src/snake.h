#ifndef SNAKE_H
#define SNAKE_H

#include "position.h"

typedef struct {
    uint max_length;
    uint length;
    Direction direction;
    Position *cells;
} Snake;

void snake_init(Snake *snake, uint max_length, uint initial_length,
                Position initial_position, Direction initial_direction);

Position snake_get_position_ahead(Snake *snake);

uint snake_is_body_cell(Snake *snake, Position position);

void snake_step(Snake *snake);

void snake_turn(Snake *snake, Direction new_direction);

void snake_grow(Snake *snake);

void snake_print(Snake *snake);

#endif /* SNAKE_H */
