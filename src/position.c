#include "position.h"
#include "utils.h"

/* amount of change on plane X for each step, indexed by Direction */
static const int XDelta[4] = {0, 1, 0, -1};

/* amount of change on plane Y for each step, indexed by Direction */
static const int YDelta[4] = {-1, 0, 1, 0};

Bool position_equal(Position p1, Position p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

Position position_random(int min_x, int min_y, int max_x, int max_y) {
    Position random;
    random.x = random_number(min_x, max_x);
    random.y = random_number(min_y, max_y);
    return random;
}

Position position_previous(Position position, Direction direction) {
    position.x -= XDelta[direction];
    position.y -= YDelta[direction];
    return position;
}

Position position_next(Position position, Direction direction) {
    position.x += XDelta[direction];
    position.y += YDelta[direction];
    return position;
}

