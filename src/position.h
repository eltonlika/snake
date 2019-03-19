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

Bool position_equal(const Position p1, const Position p2);

Position position_random(const int min_x, const int min_y, const int max_x, const int max_y);

Position position_next(const Position position, const Direction direction);

#endif /* POSITION_H */
