#ifndef POSITION_H
#define POSITION_H

#include "utils.h"

typedef enum {
    DirectionUp = 0,
    DirectionRight = 1,
    DirectionDown = 2,
    DirectionLeft = 3
} Direction;

typedef struct {
    int x;
    int y;
} Position;

Bool position_equal(Position p1, Position p2);

Position position_random(int min_x, int min_y, int max_x, int max_y);

Position position_next(Position position, Direction direction);

#endif /* POSITION_H */
