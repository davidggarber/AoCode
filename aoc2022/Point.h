#pragma once

#include <functional>

class Point
{
public:
	int64_t x, y;
	Point()
	{
		x = y = 0;
	}
	Point(int64_t xx, int64_t yy)
	{
		x = xx;
		y = yy;
	}
	Point(const Point& pt)
	{
		x = pt.x;
		y = pt.y;
	}
	const Point& operator=(const Point& pt)
	{
		x = pt.x;
		y = pt.y;
		return *this;
	}
	bool operator==(const Point& pt) const
	{
		return x == pt.x && y == pt.y;
	}
	bool operator!=(const Point& pt) const
	{
		return !operator==(pt);
	}
	bool operator<(const Point& pt) const
	{
		return y < pt.y
			|| (y == pt.y && x < pt.x);
	}
	const Point& operator+=(const Point& pt)
	{
		x += pt.x;
		y += pt.y;
		return *this;
	}
	Point operator+(const Point& pt) const
	{
		Point sum(pt.x + x, pt.y + y);
		return sum;
	}
	const Point& operator-=(const Point& pt)
	{
		x -= pt.x;
		y -= pt.y;
		return *this;
	}
	Point operator-(const Point& pt) const
	{
		Point delta(pt.x - x, pt.y - y);
		return delta;
	}
	const Point& operator*=(int k)
	{
		x *= k;
		y *= k;
		return *this;
	}
	Point operator*(int k) const
	{
		Point sum(x * k, y * k);
		return sum;
	}
	const Point& operator/=(int k)
	{
		x /= k;
		y /= k;
		return *this;
	}
	Point operator/(int k) const
	{
		Point delta(x / k, y / k);
		return delta;
	}
	const Point& operator%=(int k)
	{
		x %= k;
		y %= k;
		return *this;
	}
	Point operator%(int k) const
	{
		Point delta(x % k, y % k);
		return delta;
	}
	Point BoxRotate(int turns, int max) const
	{
		// Rotate within the box (0,0)-(max,max), inclusive
		// max MUST be positive
		turns %= 4;
		if (turns == 1 || turns == -3)
			return Point(max - y, x);  // right turn
		if (turns == 3 || turns == -1)
			return Point(y, max - x);  // left turn
		if (turns == 2 || turns == -2)
			return Point(max - x, max - y);  // U-turn
		return *this;  // no-op
	}
	Point operator>>(int turns) const
	{
		turns %= 4;
		if (turns == 1 || turns == -3)
			return Point(-y, x);  // right turn
		if (turns == 3 || turns == -1)
			return Point(y, -x);  // left turn
		if (turns == 2 || turns == -2)
			return Point(-x, -y);  // U-turn
		return *this;  // no-op
	}
	Point operator<<(int turns) const
	{
		return operator>>(4 - turns);
	}
	const Point& operator>>=(int turns)
	{
		auto xx = x, yy = y;
		turns %= 4;
		if (turns == 1 || turns == -3)
		{
			x = -yy;  // right turn
			y = xx;
		}
		else if (turns == 3 || turns == -1)
		{
			x = yy;  // left turn
			y = -xx;
		}
		else if (turns == 2 || turns == -2)
		{
			x = -xx;  // U-turn
			y = -yy;
		}
		// else no-op
		return *this;
	}
	const Point& operator<<=(int turns)
	{
		return operator>>=(4 - turns);
	}

	int64_t ManhattenDistance(const Point& pt) const
	{
		return abs(pt.x - x) + abs(pt.y - y);
	}

	static Point WEST, EAST, NORTH, SOUTH;
	static Point LEFT, RIGHT, UP, DOWN;  // redundant
};

template<>
struct std::hash<Point>
{
	size_t operator()(const Point& p) const noexcept
	{
		return (size_t)(p.x * 1000 + p.y);
	}
};
