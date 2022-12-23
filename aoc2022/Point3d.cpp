#include "Point3d.h"

// Construct static unit axis vectors
Point3d Point3d::WEST(-1, 0, 0);
Point3d Point3d::EAST(1, 0, 0);
Point3d Point3d::NORTH(0, -1, 0);
Point3d Point3d::SOUTH(0, 1, 0);
Point3d Point3d::UP(0, 0, -1);
Point3d Point3d::DOWN(0, 0, 1);
