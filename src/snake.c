#include "snake.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static const unsigned int SNAKE_INITIAL_CAPACITY = 4;

void snake_init(Snake *snake, Position initial_position, Direction initial_direction) {
    if (snake->cells != NULL) {
        free(snake->cells);
    }
    snake->cells = (Position *)malloc(sizeof(Position) * SNAKE_INITIAL_CAPACITY);
    ASSERT_ALLOC(snake->cells);
    snake->cells[0] = initial_position;
    snake->max_length = SNAKE_INITIAL_CAPACITY;
    snake->length = 1;
    snake->direction = initial_direction;
}

void snake_free(Snake *snake) {
    if (snake != NULL && snake->cells != NULL) {
        free(snake->cells);
    }
}

Bool snake_occupies_position(Snake *snake, Position position) {
    const Position *snake_cells = snake->cells;
    unsigned int snake_length = snake->length, idx;
    for (idx = 0; idx < snake_length; idx++) {
        if (position_equal(snake_cells[idx], position)) {
            return True;
        }
    }
    return False;
}

void snake_grow(Snake *snake) {
    /* if exceeding max_length then reallocate current max capacity + SNAKE_INITIAL_CAPACITY */
    if (snake->length >= snake->max_length) {
        snake->max_length += SNAKE_INITIAL_CAPACITY;
        snake->cells = realloc(snake->cells, sizeof(Position) * snake->max_length);
        ASSERT_ALLOC(snake->cells);
    }
    snake->length++;
}

void snake_step_forward(Snake *snake, Position new_head_cell) {
    /* copy existing snake cells except last cell, over the tail of snake */
    memmove(snake->cells + 1, snake->cells, sizeof(Position) * (snake->length - 1));
    /* set new head cell */
    snake->cells[0] = new_head_cell;
}

void snake_turn(Snake *snake, Direction new_direction) {
    const Direction current_direction = snake->direction;
    if (/* UP or DOWN direction changes are allowed only on LEFT or RIGHT existing directions */
        ((new_direction == DirectionUp || new_direction == DirectionDown) &&
         (current_direction == DirectionLeft || current_direction == DirectionRight)) ||
        /* LEFT or RIGHT direction changes are allowed only on UP or DOWN existing directions */
        ((new_direction == DirectionLeft || new_direction == DirectionRight) &&
         (current_direction == DirectionUp || current_direction == DirectionDown))) {
        /* change to new direction if conditions above hold */
        snake->direction = new_direction;
    }
}
