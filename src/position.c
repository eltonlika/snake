#include "position.h"
#include "utils.h"

/* amount of change on plane X for each step, indexed by Direction */
static const int XDelta[4] = {0, 1, 0, -1};

/* amount of change on plane Y for each step, indexed by Direction */
static const int YDelta[4] = {-1, 0, 1, 0};

Bool position_equal(const Position p1, const Position p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

Position position_random(const int min_x, const int min_y, const int max_x, const int max_y)
{
    Position random;
    random.x = random_number(min_x, max_x);
    random.y = random_number(min_y, max_y);
    return random;
}

Position position_next(const Position position, const Direction direction)
{
    Position next_position;
    next_position.x = position.x + XDelta[direction];
    next_position.y = position.y + YDelta[direction];
    return next_position;
}
