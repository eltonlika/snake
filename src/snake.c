#include "snake.h"
#include "utils.h"
#include <stdlib.h>

static const unsigned int BUFFER_CAPACITY = 4;

void snake_init(Snake *snake, Position initial_position, Direction initial_direction) {
    Position *cells = (Position *)malloc(sizeof(Position) * BUFFER_CAPACITY);
    ASSERT_ALLOC(cells);

    cells[0] = initial_position;
    snake->max_length = BUFFER_CAPACITY;
    snake->length = 1;
    snake->direction = initial_direction;
    snake->cells = cells;
}

Position snake_get_next_head_position(Snake *snake) {
    return position_next(snake->cells[0], snake->direction);
}

Bool snake_occupies_position(Snake *snake, Position position) {
    const unsigned int snake_last_cell_idx = snake->length - 1;
    const Position *snake_cells = snake->cells;
    unsigned int idx;
    for (idx = 0; idx <= snake_last_cell_idx; idx++) {
        if (position_equal(snake_cells[idx], position)) {
            return True;
        }
    }
    return False;
}

void snake_step_forward(Snake *snake) {
    const unsigned int snake_last_cell_idx = snake->length - 1;
    Position *snake_cells = snake->cells;
    unsigned int idx;
    for (idx = snake_last_cell_idx; idx > 0; idx--) {
        snake_cells[idx] = snake_cells[idx - 1];
    }
    snake_cells[0] = snake_get_next_head_position(snake);
}

void snake_grow(Snake *snake) {
    const unsigned int max_length = snake->max_length;
    const unsigned int current_length = snake->length;
    const unsigned int last_cell_idx = current_length - 1;
    const unsigned int new_cell_idx = current_length;
    Position *cells = snake->cells;
    const Position last_cell = cells[last_cell_idx];
    const Position new_cell = position_previous(last_cell, snake->direction);
    unsigned int new_max_length;
    Position *new_cells_buffer;

    /* if exceeding max_length then reallocate BUFFER_CAPACITY + current max capacity */
    if (current_length >= max_length) {
        new_max_length = max_length + BUFFER_CAPACITY;
        new_cells_buffer = realloc(cells, sizeof(Position) * new_max_length);
        ASSERT_ALLOC(new_cells_buffer);

        snake->max_length = new_max_length;
        snake->cells = new_cells_buffer;
    }

    snake->cells[new_cell_idx] = new_cell;
    snake->length = current_length + 1;
}

void snake_turn(Snake *snake, Direction new_direction) {
    const Direction current_direction = snake->direction;
    if (/* UP or DOWN direction changes are allowed only on LEFT or RIGHT existing directions */
        ((new_direction == DirectionUp || new_direction == DirectionDown) &&
         (current_direction == DirectionLeft || current_direction == DirectionRight)) ||
        /* LEFT or RIGHT direction changes are allowed only on UP or DOWN existing directions */
        ((new_direction == DirectionLeft || new_direction == DirectionRight) &&
         (current_direction == DirectionUp || current_direction == DirectionDown))) {
        /* change to new direction */
        snake->direction = new_direction;
    }
}
