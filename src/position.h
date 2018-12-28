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
    uint x;
    uint y;
} Position;

uint position_equal(Position p1, Position p2);

Position position_random(uint width, uint height);

Position position_previous(Position position, Direction direction);

Position position_next(Position position, Direction direction);

#endif /* POSITION_H */
