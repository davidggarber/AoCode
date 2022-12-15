#pragma once

#include <functional>

class Point
{
public:
	int x, y;
	Point()
	{
		x = y = 0;
	}
	Point(int r, int c)
	{
		y = r;
		x = c;
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
};

template<>
struct std::hash<Point>
{
	size_t operator()(const Point& p) const noexcept
	{
		return (size_t)(p.x * 1000 + p.y);
	}
};
