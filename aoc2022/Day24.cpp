#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>
#include "Point.h"
#include "Rect.h"
#include "Point3d.h"

using namespace std;

// https://adventofcode.com/2022/day/24
namespace Day24
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
        vector<string> map;
        Rect valley;
        int loop_rate;
        Point start;
        Point exit;

        Data()
        {

        }

        void Init()
        {
            blizzards.clear();

            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                string line;
                int y = 0;
                while (!file.eof())
                {
                    getline(file, line);
                    map.push_back(line);
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

            start = Point(1, 0);
            exit = Point(valley.Right() - 1, valley.Bottom());
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

        char MapAt(const Point& pos)
        {
            return map[pos.y][pos.x];
        }

        bool IsFree(const Point& pos, int minute)
        {
            //if (pos == exit)
            //    return true;
            if (!valley.Contains(pos))
                return false;
            Point e = valley.Wrap(pos - (Point::EAST * minute));
            Point w = valley.Wrap(pos - (Point::WEST * minute));
            Point n = valley.Wrap(pos - (Point::NORTH * minute));
            Point s = valley.Wrap(pos - (Point::SOUTH * minute));
            return MapAt(e) != '>'
                && MapAt(w) != '<'
                && MapAt(n) != '^'
                && MapAt(s) != 'v';
        }
    };

    Data data;

    class Progress
    {
    public:
        Point pos;
        int minute;
        int score;  // low is better

        Progress(Point p, int min, bool aim_for_exit)
            : pos(p)
            , minute(min)
        {
            score = min + pos.ManhattenDistance(aim_for_exit ? data.exit : data.start);
        }

        Progress(const Progress& p)
            : pos(p.pos)
            , minute(p.minute)
            , score(p.score)
        {
        }

        const Progress& operator=(const Progress& p)
        {
            pos = p.pos;
            minute = p.minute;
            score = p.score;
            return *this;
        }

        bool operator==(const Progress& p) const
        {
            return pos == p.pos
                && minute == p.minute;
        }

        // priority queue defers less-than objects
        bool operator<(const Progress& p) const
        {
            return score > p.score
                || (score == p.score && minute > p.minute);
        }

        vector<Progress> Next(bool aim_for_exit)
        {
            vector<Progress> next;
            if (aim_for_exit && (pos + Point::SOUTH == data.exit))
                next.push_back(Progress(pos + Point::SOUTH, minute + 1, aim_for_exit));
            else if (!aim_for_exit && (pos + Point::NORTH == data.start))
                next.push_back(Progress(pos + Point::NORTH, minute + 1, aim_for_exit));

            if (data.IsFree(pos + Point::SOUTH, minute + 1))
                next.push_back(Progress(pos + Point::SOUTH, minute + 1, aim_for_exit));
            if (data.IsFree(pos + Point::EAST, minute + 1))
                next.push_back(Progress(pos + Point::EAST, minute + 1, aim_for_exit));
            if (data.IsFree(pos + Point::WEST, minute + 1))
                next.push_back(Progress(pos + Point::WEST, minute + 1, aim_for_exit));
            if (data.IsFree(pos + Point::NORTH, minute + 1))
                next.push_back(Progress(pos + Point::NORTH, minute + 1, aim_for_exit));
            if (pos == data.start || pos == data.exit || data.IsFree(pos, minute + 1))
                next.push_back(Progress(pos, minute + 1, aim_for_exit));
            return next;
        }

        size_t Hash() const
        {
            return (pos.x << 20)
                ^ (pos.y << 10)
                ^ minute;
        }
    };

    struct ProgressHash
    {
        size_t operator()(const Progress& p) const noexcept
        {
            return p.Hash();
        }
    };

    size_t Part1()
    {
        data.Init();
        priority_queue<Progress> queue;
        unordered_set<Progress, ProgressHash> set;
        queue.push(Progress(data.start, 0, true));
        while (queue.size() > 0)
        {
            Progress p = queue.top();
            queue.pop();
            if (p.pos == data.exit)
                return p.minute;
            auto next = p.Next(true);
            for (auto it = next.begin(); it != next.end(); it++)
            {
                if (set.find(*it) == set.end())
                {
                    queue.push(*it);
                    set.insert(*it);
                }
            }
        }

        return 0;
    }

    size_t Part2()
    {
        data.Init();
        priority_queue<Progress> queue;
        unordered_set<Progress, ProgressHash> set;
        queue.push(Progress(data.start, 0, true));
        int stage = 0;
        while (queue.size() > 0)
        {
            Progress p = queue.top();
            queue.pop();
            if (stage == 2 && p.pos == data.exit)
                return p.minute;
            else if ((stage == 0 && p.pos == data.exit) || (stage == 1 && p.pos == data.start))
            {
                cout << "Stage " << stage << " completed at minute " << p.minute << "\n";
                while (!queue.empty())
                    queue.pop();
                set.clear();
                stage++;
            }
            auto next = p.Next(stage != 1);
            for (auto it = next.begin(); it != next.end(); it++)
            {
                if (set.find(*it) == set.end())
                {
                    queue.push(*it);
                    set.insert(*it);
                }
            }
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
            Stats timer(DayFromNamespace(__FUNCSIG__), 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(DayFromNamespace(__FUNCSIG__), 2);
            cout << Part2() << "\n";
        }
    }
}