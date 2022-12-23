#pragma once

#include <functional>

class Point3d
{
public:
	long long int x, y, z;
	Point3d()
	{
		x = y = z = 0;
	}
	Point3d(long long int xx, long long int yy, long long int zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	Point3d(const Point3d& pt)
	{
		x = pt.x;
		y = pt.y;
		z = pt.z;
	}
	const Point3d& operator=(const Point3d& pt)
	{
		x = pt.x;
		y = pt.y;
		z = pt.z;
		return *this;
	}
	bool operator==(const Point3d& pt) const
	{
		return x == pt.x && y == pt.y && z == pt.z;
	}
	bool operator!=(const Point3d& pt) const
	{
		return !operator==(pt);
	}
	bool operator<(const Point3d& pt) const
	{
		return z < pt.z
			|| (z == pt.z && y < pt.y)
			|| (z == pt.z && y == pt.y && x < pt.x);
	}
	const Point3d& operator+=(const Point3d& pt)
	{
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	}
	Point3d operator+(const Point3d& pt) const
	{
		Point3d sum(pt.x + x, pt.y + y, pt.z + z);
		return sum;
	}
	const Point3d& operator-=(const Point3d& pt)
	{
		x -= pt.x;
		y -= pt.y;
		z -= pt.z;
		return *this;
	}
	Point3d operator-(const Point3d& pt) const
	{
		Point3d delta(pt.x - x, pt.y - y, pt.z - z);
		return delta;
	}
	const Point3d& operator*=(int k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
	Point3d operator*(int k) const
	{
		Point3d sum(x * k, y * k, z * k);
		return sum;
	}
	const Point3d& operator/=(int k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}
	Point3d operator/(int k) const
	{
		Point3d delta(x / k, y / k, z / k);
		return delta;
	}
	const Point3d& operator%=(int k)
	{
		x %= k;
		y %= k;
		z %= k;
		return *this;
	}
	Point3d operator%(int k) const
	{
		Point3d delta(x % k, y % k, z % k);
		return delta;
	}

	long long int ManhattenDistance(const Point3d& pt) const
	{
		return abs(pt.x - x) + abs(pt.y - y) + abs(pt.z - z);
	}

	static Point3d WEST, EAST, NORTH, SOUTH, UP, DOWN;
};

template<>
struct std::hash<Point3d>
{
	size_t operator()(const Point3d& p) const noexcept
	{
		return (size_t)(p.z * 1000000 + p.y * 1000 + p.x);
	}
};
