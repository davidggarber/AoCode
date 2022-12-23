#pragma once

#include <algorithm>
#include "Point.h"

class Rect
{
public:
	int64_t left, top;
	int64_t width, height;
	Rect()
	{
		left, top, width, height = 0;
	}
	Rect(int64_t ll, int64_t tt, int64_t ww, int64_t hh)
	{
		left = ll;
		top = tt;
		width = ww;
		height = hh;
	}
	Rect(const Point& a, const Point& b)
	{
		left = std::min(a.x, b.x);
		top = std::min(a.y, b.y);
		width = std::max(a.x, b.x) - left;
		height = std::max(a.y, b.y) - top;
	}
	Rect(const Rect& rc)
	{
		left = rc.left;
		top = rc.top;
		width = rc.width;
		height = rc.height;
	}
	const Rect& operator=(const Rect& rc)
	{
		left = rc.left;
		top = rc.top;
		width = rc.width;
		height = rc.height;
		return *this;
	}
	bool operator==(const Rect& rc) const
	{
		return left == rc.left && top == rc.top && width == rc.width && height == rc.height;
	}
	bool operator!=(const Rect& rc) const
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
	bool IsEmpty() const
	{
		return width == 0 || height == 0;
	}
	bool operator<(const Rect& rc) const  // sort in reading order
	{
		return top < rc.top
			|| (top == rc.top && left < rc.left)
			|| (top == rc.top && left == rc.left && width < rc.width)
			|| (top == rc.top && left == rc.left && width == rc.width && height < rc.height);
	}
	const Rect& operator|=(const Rect& rc)  // union
	{
		int64_t ll = std::min(left, rc.left);
		int64_t tt = std::min(top, rc.top);
		int64_t rr = std::max(Right(), rc.Right());
		int64_t bb = std::max(Bottom(), rc.Bottom());
		left = ll;
		top = tt;
		width = rr - ll;
		height = bb - tt;
		return *this;
	}
	Rect operator|(const Rect& rc) const
	{
		Rect u = *this;
		return u |= rc;
	}
	const Rect& operator|=(const Point& pt)  // union
	{
		if (IsEmpty())
		{
			left = pt.x;
			top = pt.y;
			width = height = 1;
		}
		else
		{
			int64_t ll = std::min(left, pt.x);
			int64_t tt = std::min(top, pt.y);
			int64_t rr = std::max(Right(), pt.x + 1);
			int64_t bb = std::max(Bottom(), pt.y + 1);
			left = ll;
			top = tt;
			width = rr - ll;
			height = bb - tt;
		}
		return *this;
	}
	Rect operator|(const Point& pt) const
	{
		Rect u = *this;
		return u |= pt;
	}
	bool Intersects(const Rect& rc) const
	{
		return left < rc.Right()
			&& Right() > rc.left
			&& top < rc.Bottom()
			&& Bottom() > rc.top;
	}
	bool Intersects(int64_t ll, int64_t tt, int64_t ww, int64_t hh) const
	{
		return left < (ll + ww)
			&& Right() > ll
			&& top < (tt + hh)
			&& Bottom() > tt;
	}
	const Rect& operator&=(const Rect& rc)  // intersection
	{
		int64_t ll = std::max(left, rc.left);
		int64_t tt = std::max(top, rc.top);
		int64_t rr = std::min(Right(), rc.Right());
		int64_t bb = std::min(Bottom(), rc.Bottom());
		left = ll;
		top = tt;
		width = rr > ll ? (rr - ll) : 0;
		height = bb > tt ? (bb - tt) : 0;
		return *this;
	}
	Rect operator&(const Rect& rc) const
	{
		Rect inter = *this;
		return inter &= rc;
	}
	const Rect& operator+=(const Point& pt)  // translate
	{
		left += pt.x;
		top += pt.y;
		return *this;
	}
	Rect operator+(const Point& pt) const
	{
		Rect r = *this;
		return r += pt;
	}
	const Rect& operator-=(const Point& pt)  // translate
	{
		left -= pt.x;
		top -= pt.y;
		return *this;
	}
	Rect operator-(const Point& pt) const
	{
		Rect r = *this;
		return r -= pt;
	}

	int64_t Area() const
	{
		return width * height;
	}
};

template<>
struct std::hash<Rect>
{
	size_t operator()(const Rect& r) const noexcept
	{
		return (size_t)(r.left * 1000000 + r.top * 10000 + r.width * 100 + r.height);
	}
};
