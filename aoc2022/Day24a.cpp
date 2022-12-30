#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "Point.h"
#include "Rect.h"
#include "Point3d.h"

using namespace std;

// https://adventofcode.com/2022/day/24
namespace Day24a
{
    class Blizzard
    {
    public:
        Point pos, dir;

        Blizzard(int x, int y, char c)
            : pos(x, y)
        {
            if (c == '<')
                dir = Point::WEST;
            else if (c == '>')
                dir = Point::EAST;
            else if (c == '^')
                dir = Point::NORTH;
            else if (c == 'v')
                dir = Point::SOUTH;
        }

        Blizzard(const Blizzard& b) = default;
        const Blizzard& operator=(const Blizzard& b)
        {
            pos = b.pos;
            dir = b.dir;
        }

        void Move(const Rect& valley)
        {
            pos += dir;
            if (!valley.Contains(pos))
                pos = valley.Wrap(pos);
        }

        Point Future(const Rect& valley, int steps)
        {
            Point p = pos + dir * steps;
            if (!valley.Contains(p))
                p = valley.Wrap(p);
            return p;
        }
    };

    class Data
    {
    public:
        vector<Blizzard> blizzards;
        Rect valley;
        int loop_rate;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                string line;
                int y = 0;
                while (!file.eof())
                {
                    getline(file, line);
                    for (int x = 0; x < line.size(); x++)
                    {
                        char c = line[x];
                        if (c == '>' || c == '<' || c == '^' || c == 'v')
                            blizzards.push_back(Blizzard(x, y, c));
                    }
                    y++;
                }
                valley.left = valley.top = 1;
                valley.height = y - 2;
                valley.width = line.size() - 2;

                int h = valley.height, w = valley.width;
                int f = 2;
                while (f <= h)
                {
                    while ((h % f == 0) && (w % f) == 0)
                    {
                        h /= f;
                        w /= f;
                    }
                    f++;
                }
                loop_rate = h * valley.width;
            }
        }

        unordered_set<Point> BlizzardsAt(int minute)
        {
            unordered_set<Point> map;
            for (auto it = blizzards.begin(); it != blizzards.end(); it++)
            {
                map.insert(it->Future(valley, minute));
            }
            return std::move(map);
        }

        Point Exit()
        {
            return Point(valley.Right() - 1, valley.Bottom());
        }
    };

    class Route
    {
    public:
        //vector<Point> steps;
        Rect valley;
        Point start;
        Point cur;
        Point loop_detect;
        int loop_minute;
        unordered_set<Point>& blizzards;
        unordered_set<Point3d>& seen;

        Route(const Rect& v, Point c, int min, unordered_set<Point>& bliz, unordered_set<Point3d>& sn)
            : start(1, 0)
            , valley(v)
            , cur(c)
            , loop_minute(min)
            , blizzards(bliz)
            , seen(sn)
        {
        }

        Point Exit()
        {
            return Point(valley.Right() - 1, valley.Bottom());
        }

        Point Cur()
        {
            return cur;
            //Point p(1, 0);  // Always
            //for (auto & dir : steps)
            //{
            //    p += dir;
            //}
        }

        bool AtExit()
        {
            return Cur() == Exit();
        }

        bool IsSafe(const Point& p)
        {
            Point3d p3(p.x, p.y, loop_minute);
            if (seen.find(p3) != seen.end())
                return false;  // Do not go back here
            return valley.Contains(p)
                && find(blizzards.begin(), blizzards.end(), p) == blizzards.end();
        }

        vector<Point> Moves()
        {
            vector<Point> moves;
            Point c = Cur();
            if (c + Point::SOUTH == Exit())
            {
                moves.push_back(Point::SOUTH);
                return moves;
            }
            if (c == start || IsSafe(c))
                moves.push_back(Point::ZERO);  // Can sit still
            if (IsSafe(c + Point::NORTH))
                moves.push_back(Point::NORTH);
            if (IsSafe(c + Point::SOUTH))
                moves.push_back(Point::SOUTH);
            if (IsSafe(c + Point::WEST))
                moves.push_back(Point::WEST);
            if (IsSafe(c + Point::EAST))
                moves.push_back(Point::EAST);

            return moves;
        }
    };

    class Path
    {
    public:
        vector<vector<Point>> path;

        int Minute()
        {
            return path.size();
        }

        Point Cur()
        {
            Point p(1, 0);  // Always
            for (auto & step : path)
            {
                Point dir = step.back();
                p += dir;
            }
            return p;
        }

        Point LoopDetector(int loop_rate)
        {
            Point p(1, 0);  // Always
            for (int i = 0; i < (int)path.size() + 1 - loop_rate; i++)
            {
                Point dir = path[i].back();
                p += dir;
            }
            return p;
        }

        void Move(vector<Point>& moves)
        {
            path.push_back(moves);
        }

        void RollBack()
        {
            auto it = path.end();
            while (path.size() > 0)
            {
                auto& step = path.back();
                step.pop_back();
                if (step.size() > 0)
                    break;
                path.pop_back();
            }
        }

        void Print()
        {
            cout << path.size() << " steps: ";
            Point p(1, 0);  // Always
            for (auto& step : path)
            {
                Point dir = step.back();
                p += dir;
                if (dir.x > 0)
                    cout << "right, ";
                else if (dir.x < 0)
                    cout << "left, ";
                else if (dir.y > 0)
                    cout << "down, ";
                else if (dir.y < 0)
                    cout << "up, ";
                else
                    cout << "sit, ";
            }
            cout << " == " << p.x << "," << p.y << "\n";
        }
    };

    size_t Part1()
    {
        Data data;
        Path path;
        int best = 0x1 << 30;
        Point exit = data.Exit();
        unordered_set<Point3d> seen;

        while (true)
        {
            Point cur = path.Cur();
            //path.Print();

            Point3d curM(cur.x, cur.y, path.Minute() % data.loop_rate);
            seen.insert(curM);

            if (cur == exit)
            {
                if (path.Minute() < best)
                {
                    best = path.Minute();
                    cout << "Found a route in " << best << " minutes\n";
                    //path.Print();
                    //auto debug = data.BlizzardsAt(11);
                }
            }
            auto blizzards = data.BlizzardsAt(path.Minute() + 1);
            Route r(data.valley, cur, (path.Minute() + 1) % data.loop_rate, blizzards, seen);
            auto next = r.Moves();
            if (next.size() == 0 || path.Minute() + cur.ManhattenDistance(exit) >= best)
            {
                path.RollBack();
                if ((path.Minute() % 20) == 0)
                    cout << "Roll back to " << path.Minute() << "\n";
                if (path.Minute() == 0)
                    break;
            }
            else
                path.path.push_back(next);
        }

        // UNSOLVED
        // Currently takes about 40 minutes. 
        // Best guess was 413, but AoC says that's still too high.

        return best;
    }

    size_t Part2()
    {
        Data data;
        return 0;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(DayFromNamespace(__FUNCSIG__), 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(DayFromNamespace(__FUNCSIG__), 2);
            cout << Part2() << "\n";
        }
    }
}