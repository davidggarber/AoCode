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
	Point operator-(const Point& pt) const
	{
		Point delta(pt.y - y, pt.x - x);
		return delta;
	}
	int64_t ManhattenDistance(const Point& pt) const
	{
		return abs(pt.x - x) + abs(pt.y - y);
	}
};

template<>
struct std::hash<Point>
{
	size_t operator()(const Point& p) const noexcept
	{
		return (size_t)(p.x * 1000 + p.y);
	}
};
