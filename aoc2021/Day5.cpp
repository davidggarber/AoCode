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
        // (This assumes p is on the line and this < p)
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

        Segment() = default;

        // Single-point segment
        Segment(int x, int y) noexcept
            : start(x, y), dir(0,1), len(0)
        {
        }

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

        // Find any intersections between one segment and all prior
        // Add them to a set for unique tracking
        void PushIntersections(PointSet& intersections, vector<Segment>::iterator it, int count) const
        {
            for (int i = 0; i < count; i++, it++)
            {
                Segment overlap;
                if (GetOverlap(*it, overlap))
                {
                    for (int j = 0; j <= overlap.len; j++)
                    {
                        Point p = overlap.start.Offset(overlap.dir, j);
                        intersections.insert(overlap.start.Offset(overlap.dir, j));
                    }
                }
            }
        }

        // Is this point on the line defined by this segment 
        // (even if beyond the ends)
        bool PointOnLine(const Point& p) const
        {
            if (dir.x == 0)
                return p.x == start.x;
            if (dir.y == 0)
                return p.y == start.y;
            // We're diagonal. Offsets must be the same for x and y
            int dx = (p.x - start.x) / dir.x;
            int dy = (p.y - start.y) / dir.y;
            return dx == dy;
        }

        // Get the overlap between this segment and another.
        // The result is a third segment (which can be a point or a segment)
        // Return false if no overlap exists
        bool GetOverlap(const Segment& o, Segment& lap) const
        {
            // Always ask the left/upper segment about the lower/right one
            if (o.start < start)
                return o.GetOverlap(*this, lap);

            if (dir == o.dir)
            {
                // Parallel
                if (!PointOnLine(o.start))
                    return false;

                lap.dir = dir;
                int t = start.Len(o.start);
                if (t > len || t < 0)
                    return false;

                // Two parallel segments can overlap along multiple points
                lap.start = start.Offset(dir, t);
                lap.len = min(o.len, len - t);
                return true;
            }

            // Two non-parallel segments can overlap on at most 1 point
            lap.len = 0;
            lap.dir = dir;  // direction doesn't matter when len==0

            if (dir.x == 0)
            {
                // We are vertical. Find where o crosses
                // e+ft=a
                int t = (start.x - o.start.x) / o.dir.x;
                if (t < 0 || t > o.len)
                    return false;
                lap.start = o.start.Offset(o.dir, t);
                int u = (lap.start.y - start.y) / dir.y;
                return u >= 0 && u <= len;
            }
            if (o.dir.x == 0)
            {
                // O is vertical. Find where we cross
                // a+bt=e
                int t = (o.start.x - start.x) / dir.x;
                if (t < 0 || t > len)
                    return false;
                lap.start = start.Offset(dir, t);
                int u = (lap.start.y - o.start.y) / o.dir.y;
                return u >= 0 && u <= o.len;
            }
            else
            {
                // Neither is vertical. Both have simple slopes
                // y = mx + b
                int m = dir.y / dir.x;
                int om = o.dir.y / o.dir.x;
                // m != om, because lines aren't parallel
                int b = start.y - m * start.x;
                int ob = o.start.y - om * o.start.x;
                // Given the Y-intercepts and the slopes, there is a single y where they meet.
                // At what x does that happen?
                // mx+b = omx+ob
                // mx-omx = ob-b
                int x = (ob - b) / (m - om);
                // Does that x happen within both of their ranges?
                int t = (x - start.x) / dir.x;
                int u = (x - o.start.x) / o.dir.x;
                if (t < 0 || t > len || u < 0 || u > o.len)
                    return false;
                // Do those xs really map to the same y?
                lap.start = start.Offset(dir, t);
                Point p = o.start.Offset(o.dir, u);
                return lap.start == p;
            }
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

        return intersections.size();
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