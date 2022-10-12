#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <sstream>
#include "Stopwatch.h"

using namespace std;

// https://adventofcode.com/2021/day/5
namespace Day5
{
    auto data_file = "Data\\Day5.txt";

    class Point
    {
    public:
        int x, y;

        Point() = default;
        Point(const Point& p) = default;

        Point(int xx, int yy) noexcept
        {
            x = xx;
            y = yy;
        }

        const Point& operator=(const Point& p)
        {
            x = p.x;
            y = p.y;
            return *this;
        }

        //Point() noexcept
        //{
        //    x = y = 0;
        //}

        //Point(const Point& p) noexcept
        //{
        //    x = p.x;
        //    y = p.y;
        //}

        bool operator==(const Point& p) const
        {
            return x == p.x && y == p.y;
        }

        // Sort in reading order
        bool operator<(const Point& p) const
        {
            return y < p.y
                || (y == p.y && x < p.x);
        }

        // Rotate
        Point operator~() const
        {
            Point r;
            r.x = y;
            r.y = -x;
            return r;
        }

        // A cartesion unit vector pointing from us to p
        Point Dir(const Point& p) const
        {
            Point d;
            d.x = p.x > x ? 1 : p.x < x ? -1 : 0;
            d.y = p.y > y ? 1 : p.y < y ? -1 : 0;
            if (d.x == 0 && d.y == 0)
                d.y = 1;  // must have a unit vector
            return std::move(d);
        }

        // The distance from here to p along the unit vector
        int Len(const Point& p) const
        {
            if (x != p.x)
                return abs(p.x - x);
            return abs(p.y - y);
        }

        // Find the offset from this point, given a direction and number of steps
        Point Offset(const Point& dir, int len) const
        {
            Point b;
            b.x = x + dir.x * len;
            b.y = y + dir.y * len;
            return std::move(b);
        }

        void Read(ifstream& file)
        {
            // Two numbers separated by a comma. No spaces.
            char comma;
            file >> x;
            file >> comma;
            file >> y;
        }
    };

    // Custom std::hash specialization for Point
    struct PointHash
    {
        size_t operator()(const Point& p) const noexcept 
        {
            return (size_t)(p.x * 1000 + p.y);
        }
    };

    typedef unordered_set<Point, PointHash> PointSet;

    class Segment
    {
    public:
        Point start, dir;
        int len;  // zero is a point, negative is invalid

        Segment() noexcept
        {
        }

        // Single-point segment
        Segment(int x, int y) noexcept
            : start(x, y), dir(0,1), len(0)
        {
        }

        bool Empty() const { return len >= 0; }
        int Left() const { return min(start.x, start.x + dir.x * len); }
        int Right() const { return max(start.x, start.x + dir.x * len); }
        int Top() const { return start.y; }
        int Bottom() const { return start.y + dir.y * len; }

        bool Read(ifstream& file)
        {
            if (file.eof())
                return false;
            // Each line has the format
            // 111,222 -> 333,444            
            Point a, b;
            a.Read(file);
            char c;  // stream >> will skip spaces, but not other punctuation
            file >> c >> c;  // ->
            b.Read(file);
            
            if (a < b)
            {
                start = a;
                dir = a.Dir(b);
                len = a.Len(b);
            }
            else
            {
                start = b;
                dir = b.Dir(a);
                len = b.Len(a);
            }
            
            return true;
        }
        void PushIntersections(PointSet& intersections, vector<Segment>::iterator it, int count) const
        {
            for (int i = 0; i < count; i++, it++)
            {
                Segment overlap;
                if (GetOverlap(*it, overlap))
                {
                    for (int j = 0; j <= overlap.len; j++)
                        intersections.insert(overlap.start.Offset(overlap.dir, j));
                }
            }
        }

        // Do the bounding rectangles of the two segments intersect?
        bool IntersectsRect(const Segment& s) const
        {
            return Left() <= s.Right()
                && Right() >= s.Left()
                && Top() <= s.Bottom()
                && Bottom() >= s.Top();
        }

        bool GetOverlap(const Segment& o, Segment& lap) const
        {
            // Always ask the left/upper segment about the lower/right one
            if (o.start < start)
                return o.GetOverlap(*this, lap);

            if (dir == o.dir)
            {
                // Parallel
                lap.dir = dir;

                // We know this is before o, so o-this must be >= 0. -1 indicates a miss
                int t = dir.x != 0 ? ((o.start.x - start.x) / dir.x) : o.start.x == start.x ? 0 : -1;
                int u = dir.y != 0 ? ((o.start.y - start.y) / dir.y) : o.start.y == start.y ? 0 : -1;
                if (t != u && t != 0 && u != 0)
                    return false;
                if (t == 0)
                    t = u;
                if (t > len || t < 0)
                    return false;

                // Two parallel segments can overlap along multiple points
                lap.start = start.Offset(dir, t);
                lap.len = min(o.len, len - t);
                return true;
            }
            if (dir == ~o.dir || ~dir == o.dir)
            {
                // Perpendicular
                if (dir.x == 0 || dir.y == 0)
                {
                    // Simple horizontal & vertical
                    if (!IntersectsRect(o))
                        return false;

                    // Two non-parallel segments can overlap on at most 1 point
                    lap.dir.y = 1;
                    lap.len = 0;
                    lap.start.x = (dir.x == 0) ? start.x : o.start.x;
                    lap.start.y = (dir.y == 0) ? start.y : o.start.y;
                    return true;
                }
                else
                {
                    // This is the only case where IntersectsRect can be wrong
                    int dx = (o.start.x - start.x) / dir.x;
                    int dy = (o.start.y - start.y) / dir.y;
                    if (!(dx & 0x1) || !(dy & 0x1))
                        return false;
                    // TODO
                }
            }
            else
            {
                // Intersections at 45 or 135 degrees
                // At most 1 point of intersection
                if (!IntersectsRect(o))  // This works again
                    return false;
                // TODO
            }

            return false;
        }
    };

    class Data
    {
    public:
        vector<Segment> segments;

        Data(bool diagonals)
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                while (!file.eof())
                {
                    Segment s;
                    if (s.Read(file))
                    {
                        if (diagonals || s.dir.x == 0 || s.dir.y == 0)
                            segments.push_back(std::move(s));
                    }
                }
            }
        }
    };

    size_t Part1()
    {
        Data data(false);  // only horizontals and verticals

        PointSet intersections;
        for (int i = 1; i < data.segments.size(); i++)
        {
            data.segments[i].PushIntersections(intersections, data.segments.begin(), i);
        }

        return intersections.size();
    }

    size_t Part2()
    {
        Data data(true);  // diagonals too

        PointSet intersections;
        for (int i = 1; i < data.segments.size(); i++)
        {
            data.segments[i].PushIntersections(intersections, data.segments.begin(), i);
        }

        return 0;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(5, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(5, 2);
            cout << Part2() << "\n";
        }
    }
}