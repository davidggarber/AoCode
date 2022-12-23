#pragma once

#include <algorithm>
#include "Point3d.h"

class Box3d
{
public:
	int64_t left, top, front;
	int64_t width, height, depth;
	Box3d()
		: left(0)
		, top(0)
		, front(0)
		, width(0)
		, height(0)
		, depth(0)
	{	
	}
	Box3d(int64_t ll, int64_t tt, int64_t ff, int64_t ww, int64_t hh, int64_t dd)
		: left(ll)
		, top(tt)
		, front(ff)
		, width(ww)
		, height(hh)
		, depth(dd)
	{
	}
	Box3d(const Point3d& a, const Point3d& b)
		: left(std::min(a.x, b.x))
		, top(std::min(a.y, b.y))
		, front(std::min(a.z, b.z))
		, width(std::max(a.x, b.x) - left)
		, height(std::max(a.y, b.y) - top)
		, depth(std::max(a.z, b.z) - front)
	{
	}
	Box3d(const Box3d& rc)
		: left(rc.left)
		, top(rc.top)
		, width(rc.width)
		, height(rc.height)
	{
	}
	const Box3d& operator=(const Box3d& rc)
	{
		left = rc.left;
		top = rc.top;
		front = rc.front;
		width = rc.width;
		height = rc.height;
		depth = rc.depth;
		return *this;
	}
	bool operator==(const Box3d& rc) const
	{
		return left == rc.left && top == rc.top && front == rc.front
			&& width == rc.width && height == rc.height && depth == rc.depth;
	}
	bool operator!=(const Box3d& rc) const
	{
		return !operator==(rc);
	}
	int64_t Right() const
	{
		return left + width;
	}
	int64_t Bottom() const
	{
		return top + height;
	}
	int64_t Back() const
	{
		return front + depth;
	}
	bool IsEmpty() const
	{
		return width == 0 || height == 0 || depth == 0;
	}
	bool operator<(const Box3d& rc) const  // sort in reading order
	{
		return front < rc.front
			|| (front == rc.front && top < rc.top)
			|| (front == rc.front && top == rc.top && left < rc.left)
			|| (front == rc.front && top == rc.top && left == rc.left && depth < rc.depth)
			|| (front == rc.front && top == rc.top && left == rc.left && depth == rc.depth && width < rc.width)
			|| (front == rc.front && top == rc.top && left == rc.left && depth == rc.depth && width == rc.width && height < rc.height);
	}
	const Box3d& operator|=(const Box3d& rc)  // union
	{
		int64_t ll = std::min(left, rc.left);
		int64_t tt = std::min(top, rc.top);
		int64_t ff = std::min(front, rc.front);
		int64_t rr = std::max(Right(), rc.Right());
		int64_t bb = std::max(Bottom(), rc.Bottom());
		int64_t dd = std::max(Back(), rc.Back());
		left = ll;
		top = tt;
		front = ff;
		width = rr - ll;
		height = bb - tt;
		depth = dd - ff;
		return *this;
	}
	Box3d operator|(const Box3d& rc) const
	{
		Box3d u = *this;
		return u |= rc;
	}
	const Box3d& operator|=(const Point3d& pt)  // union
	{
		if (IsEmpty())
		{
			left = pt.x;
			top = pt.y;
			front = pt.z;
			width = height = depth = 1;
		}
		else
		{
			int64_t ll = std::min(left, pt.x);
			int64_t tt = std::min(top, pt.y);
			int64_t ff = std::min(front, pt.z);
			int64_t rr = std::max(Right(), pt.x + 1);
			int64_t bb = std::max(Bottom(), pt.y + 1);
			int64_t dd = std::max(Back(), pt.z + 1);
			left = ll;
			top = tt;
			front = ff;
			width = rr - ll;
			height = bb - tt;
			depth = dd - ff;
		}
		return *this;
	}
	Box3d operator|(const Point3d& pt) const
	{
		Box3d u = *this;
		return u |= pt;
	}
	bool Intersects(const Box3d& rc) const
	{
		return left < rc.Right()
			&& Right() > rc.left
			&& top < rc.Bottom()
			&& Bottom() > rc.top
			&& front < rc.Back()
			&& Back() > rc.front;
	}
	bool Intersects(int64_t ll, int64_t tt, int64_t ff, int64_t ww, int64_t hh, int64_t dd) const
	{
		return left < (ll + ww)
			&& Right() > ll
			&& top < (tt + hh)
			&& Bottom() > tt
			&& front < (ff + dd)
			&& Back() > ff;
	}
	const Box3d& operator&=(const Box3d& rc)  // intersection
	{
		int64_t ll = std::max(left, rc.left);
		int64_t tt = std::max(top, rc.top);
		int64_t ff = std::max(front, rc.front);
		int64_t rr = std::min(Right(), rc.Right());
		int64_t bb = std::min(Bottom(), rc.Bottom());
		int64_t dd = std::min(Back(), rc.Back());
		left = ll;
		top = tt;
		front = ff;
		width = rr > ll ? (rr - ll) : 0;
		height = bb > tt ? (bb - tt) : 0;
		depth = dd > ff ? (dd - ff) : 0;
		return *this;
	}
	Box3d operator&(const Box3d& rc) const
	{
		Box3d inter = *this;
		return inter &= rc;
	}
	const Box3d& operator+=(const Point3d& pt)  // translate
	{
		left += pt.x;
		top += pt.y;
		front += pt.z;
		return *this;
	}
	Box3d operator+(const Point3d& pt) const
	{
		Box3d r = *this;
		return r += pt;
	}
	const Box3d& operator-=(const Point3d& pt)  // translate
	{
		left -= pt.x;
		top -= pt.y;
		front -= pt.z;
		return *this;
	}
	Box3d operator-(const Point3d& pt) const
	{
		Box3d r = *this;
		return r -= pt;
	}

	bool Contains(const Point3d& pt) const
	{
		return pt.x >= left && pt.x < Right()
			&& pt.y >= top && pt.y < Bottom()
			&& pt.z >= front && pt.z < Back();
	}

	void Expand(const Point3d& dirs)
	{
		left -= dirs.x;
		width += dirs.x * 2;
		top -= dirs.y;
		height += dirs.y * 2;
		front -= dirs.z;
		depth += dirs.z * 2;
	}

	int64_t Volume() const
	{
		return width * height * depth;
	}
};

template<>
struct std::hash<Box3d>
{
	size_t operator()(const Box3d& r) const noexcept
	{
		return (size_t)(r.left << 52
			^ r.top << 41
			^ r.front << 30
			^ r.width << 20
			^ r.height << 10
			^ r.depth);
	}
};
