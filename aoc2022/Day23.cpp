#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "Point.h";
#include "Rect.h";

using namespace std;

// https://adventofcode.com/2022/day/23
namespace Day23
{
    class Data
    {
    public:
        unordered_set<Point> elves;

        vector<Point> priorities =
        {
            Point(0, -1),
            Point(0, 1),
            Point(-1, 0),
            Point(1, 0)
        };

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                int y = 0;
                while (!file.eof())
                {
                    string line;
                    getline(file, line);
                    for (int x = 0; x < line.size(); x++)
                    {
                        if (line[x] == '#')
                            elves.insert(Point(x, y));
                    }
                    y++;
                }
            }
        }

        bool Check(const Point& p)  // IsElf
        {
            return elves.find(p) != elves.end();
        }

        bool IsAlone(const Point& p)
        {
            return !Check(p + Point(-1, -1))
                && !Check(p + Point(0, -1))
                && !Check(p + Point(1, -1))
                && !Check(p + Point(1, 0))
                && !Check(p + Point(1, 1))
                && !Check(p + Point(0, 1))
                && !Check(p + Point(-1, 1))
                && !Check(p + Point(-1, 0));
        }

        bool IsClear(const Point& p, int dir)
        {
            dir %= 4;
            Point d = p + priorities[dir];
            dir &= 2;
            Point dl = d + priorities[(dir + 2) % 4];
            Point dr = d + priorities[(dir + 3) % 4];
            return !Check(d) && !Check(dl) && !Check(dr);
        }

        Rect Bounds() const
        {
            Rect bounds;
            for (auto it = elves.begin(); it != elves.end(); it++)
            {
                bounds |= *it;
            }
            return bounds;
        }

        void Paint(const Rect& bounds) const
        {
            char* map = new char[bounds.Area() + 1];
            memset(map, 0, bounds.Area() + 1);
            for (int i = bounds.Area() - 1; i >= 0; i--)
                map[i] = '.';

            for (auto it = elves.begin(); it != elves.end(); it++)
            {
                int x = it->x - bounds.left;
                int y = it->y - bounds.top;
                map[y * bounds.width + x] = '#';
            }
            string s(map);
            for (int i = 0; i < bounds.height; i++)
            {
                cout << s.substr(i * bounds.width, bounds.width) << "\n";
            }
            cout << "\n";
        }
    };

    size_t Part1()
    {
        Data data;
        //Rect bounds0 = data.Bounds();
        //data.Paint(bounds0);

        for (int round = 0; round < 10; round++)
        {
            unordered_map<Point, Point> proposals;  // src -> dest
            unordered_map<Point, Point> movers;  // dest -> src
            for (auto it = data.elves.begin(); it != data.elves.end(); it++)
            {
                proposals[*it] = *it;  // will get overwritten
                if (!data.IsAlone(*it))
                {
                    int pri = round % 4;
                    for (int i = pri; i < pri + 4; i++)
                    {
                        if (data.IsClear(*it, i))
                        {
                            Point dest = *it + data.priorities[i % 4];
                            if (movers.find(dest) == movers.end())
                            {
                                proposals[*it] = dest;
                                movers[dest] = *it;
                            }
                            else // collision
                            {
                                proposals[*it] = *it;
                                proposals[movers[dest]] = movers[dest];
                            }
                            break;
                        }
                    }
                }
            }

            data.elves.clear();
            for (auto it = proposals.begin(); it != proposals.end(); it++)
            {
                data.elves.insert(it->second);
            }

            //Rect bounds2 = data.Bounds();
            //int area = bounds2.Area();
            //cout << "Round " << (round + 1) << "\n";
            //data.Paint(bounds2);
        }

        Rect bounds = data.Bounds();
        //data.Paint(bounds);

        return bounds.Area() - data.elves.size();
    }

    size_t Part2()
    {
        Data data;
        //Rect bounds0 = data.Bounds();
        //data.Paint(bounds0);

        int round = 0;
        while (true)
        {
            unordered_map<Point, Point> proposals;  // src -> dest
            unordered_map<Point, Point> movers;  // dest -> src
            for (auto it = data.elves.begin(); it != data.elves.end(); it++)
            {
                proposals[*it] = *it;  // will get overwritten
                if (!data.IsAlone(*it))
                {
                    int pri = round % 4;
                    for (int i = pri; i < pri + 4; i++)
                    {
                        if (data.IsClear(*it, i))
                        {
                            Point dest = *it + data.priorities[i % 4];
                            if (movers.find(dest) == movers.end())
                            {
                                proposals[*it] = dest;
                                movers[dest] = *it;
                            }
                            else // collision
                            {
                                proposals[*it] = *it;
                                proposals[movers[dest]] = movers[dest];
                            }
                            break;
                        }
                    }
                }
            }

            data.elves.clear();
            for (auto it = proposals.begin(); it != proposals.end(); it++)
            {
                data.elves.insert(it->second);
            }

            //Rect bounds2 = data.Bounds();
            //int area = bounds2.Area();
            //cout << "Round " << (round + 1) << "\n";
            //data.Paint(bounds2);

            round++;
            int count = 0;
            for (auto it = proposals.begin(); it != proposals.end(); it++)
            {
                if (it->first != it->second)
                {
                    count++;
                    break;
                }
            }
            if (count == 0)
                break;
        }

        return round;
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