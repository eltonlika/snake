#include "position.h"
#include "random.h"

/* amount of change on plane X for each step, indexed by Direction */
static const int XDelta[4] = {0, 1, 0, -1};

/* amount of change on plane Y for each step, indexed by Direction */
static const int YDelta[4] = {-1, 0, 1, 0};

Bool position_equal(Position p1, Position p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

Position position_random(unsigned int width, unsigned int height) {
    Position random;
    random.x = random_number(0, width);
    random.y = random_number(0, height);
    return random;
}

Position position_previous(Position position, Direction direction) {
    Position previous;
    previous.x = position.x - XDelta[direction];
    previous.y = position.y - YDelta[direction];
    return previous;
}

Position position_next(Position position, Direction direction) {
    Position next;
    next.x = position.x + XDelta[direction];
    next.y = position.y + YDelta[direction];
    return next;
}
