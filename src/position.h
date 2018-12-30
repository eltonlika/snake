#ifndef POSITION_H
#define POSITION_H

typedef enum {
    DirectionUp = 0,
    DirectionRight = 1,
    DirectionDown = 2,
    DirectionLeft = 3
} Direction;

typedef struct {
    unsigned int x;
    unsigned int y;
} Position;

int position_equal(Position p1, Position p2);

Position position_random(unsigned int width, unsigned int height);

Position position_previous(Position position, Direction direction);

Position position_next(Position position, Direction direction);

#endif /* POSITION_H */
