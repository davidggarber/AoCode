#include "Point.h"

// Construct static unit axis vectors
Point Point::ZERO(0, 0);

Point Point::WEST(-1, 0);
Point Point::EAST(1, 0);
Point Point::NORTH(0, -1);
Point Point::SOUTH(0, 1);

Point Point::LEFT(-1, 0);
Point Point::RIGHT(1, 0);
Point Point::UP(0, -1);
Point Point::DOWN(0, 1);
