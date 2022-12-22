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
#include "Point.h"

using namespace std;

// https://adventofcode.com/2022/day/22
namespace Day22
{
    class Data
    {
    public:
        vector<string> map;
        vector<int> steps;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    if (line.size() == 0)
                        break;
                    map.push_back(line);
                }
                getline(file, line);
                int dist = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == 'R')
                    {
                        steps.push_back(dist);
                        steps.push_back(-1);
                        dist = 0;
                    }
                    else if (line[i] == 'L')
                    {
                        steps.push_back(dist);
                        steps.push_back(-2);
                        dist = 0;
                    }
                    else
                    {
                        dist = dist * 10 + (line[i] - '0');
                    }
                }
                steps.push_back(dist);
            }
        }

        int Height()
        {
            return map.size();
        }

        int Width()
        {
            return map[0].size();
        }

        char Get(const Point& p)
        {
            if (p.y >= Height() || p.y < 0 || p.x < 0)
                return ' ';
            if (p.x >= map[p.y].size())
                return ' ';
            return map[p.y][p.x];
        }

        Point Next(const Point& cur, const Point& dir)
        {
            Point p = cur;
            char ch;
            do
            {
                p = p + dir;
                if (p.x >= Width())
                    p.x -= Width();
                else if (p.x < 0)
                    p.x += Width();
                if (p.y >= Height())
                    p.y -= Height();
                else if (p.y < 0)
                    p.y += Height();
                ch = Get(p);
            } while (ch == ' ');
            return p;
        }

        Point NextFace(Point from, Point dest, int& turn)
        {
            if (Height() == 12)
            {
                from = (from + Point(4, 4)) / 4;
                dest = (dest + Point(4, 4)) / 4;
                if (from == Point(3, 1))
                {
                    if (dest == Point(3, 0))
                    {
                        turn = 2;
                        return Point(0, 1);
                    }
                    if (dest == Point(2, 1))
                    {
                        turn = -1;
                        return Point(1, 1);
                    }
                    if (dest == Point(4, 1))
                    {
                        turn = 2;
                        return Point(3, 2);
                    }
                }
                else if (from == Point(1, 2))
                {
                    if (dest == Point(1, 1))
                    {
                        turn = 2;
                        return Point(2, 0);
                    }
                    if (dest == Point(0, 2))
                    {
                        turn = 1;
                        return Point(3, 2);
                    }
                    if (dest == Point(1, 3))
                    {
                        turn = 2;
                        return Point(2, 2);
                    }
                }
                else if (from == Point(2, 2))
                {
                    if (dest == Point(2, 1))
                    {
                        turn = 1;
                        return Point(2, 0);
                    }
                    if (dest == Point(2, 3))
                    {
                        turn = -1;
                        return Point(2, 2);
                    }
                }
                else if (from == Point(3, 2))
                {
                    if (dest == Point(4, 2))
                    {
                        turn = 1;
                        return Point(3, 2);
                    }
                }
                else if (from == Point(3, 3))
                {
                    if (dest == Point(2, 3))
                    {
                        turn = 1;
                        return Point(1, 1);
                    }
                    if (dest == Point(3, 4))
                    {
                        turn = 2;
                        return Point(0, 1);
                    }
                }
                else if (from == Point(4, 3))
                {
                    if (dest == Point(4, 2))
                    {
                        turn = -1;
                        return Point(2, 1);
                    }
                    if (dest == Point(5, 3))
                    {
                        turn = 2;
                        return Point(2, 0);
                    }
                    if (dest == Point(4, 4))
                    {
                        turn = -1;
                        return Point(0, 1);
                    }
                }
                cout << "Unexpected face transition\n";
                return from;
            }
            else // (Height() == 200)
            {
                from = (from + Point(50, 50)) / 50;
                dest = (dest + Point(50, 50)) / 50;
                if (from == Point(2, 1))
                {
                    if (dest == Point(2, 0))
                    {
                        turn = 1;
                        return Point(0, 3);
                    }
                    if (dest == Point(1, 1))
                    {
                        turn = 2;
                        return Point(0, 2);
                    }
                }
                else if (from == Point(3, 1))
                {
                    if (dest == Point(3, 0))
                    {
                        //turn = 2;
                        return Point(0, 3);
                    }
                    if (dest == Point(4, 1))
                    {
                        turn = 2;
                        return Point(1, 2);
                    }
                    if (dest == Point(3, 2))
                    {
                        turn = 1;
                        return Point(1, 1);
                    }
                }
                else if (from == Point(2, 2))
                {
                    if (dest == Point(1, 2))
                    {
                        turn = -1;
                        return Point(0, 2);
                    }
                    if (dest == Point(3, 2))
                    {
                        turn = -1;
                        return Point(2, 0);
                    }
                }
                else if (from == Point(1, 3))
                {
                    if (dest == Point(1, 2))
                    {
                        turn = 1;
                        return Point(1, 1);
                    }
                    if (dest == Point(0, 3))
                    {
                        turn = 2;
                        return Point(1, 0);
                    }
                }
                else if (from == Point(2, 3))
                {
                    if (dest == Point(3, 3))
                    {
                        turn = 2;
                        return Point(2, 0);
                    }
                    if (dest == Point(2, 4))
                    {
                        turn = 1;
                        return Point(0, 3);
                    }
                }
                else if (from == Point(1, 4))
                {
                    if (dest == Point(2, 4))
                    {
                        turn = -1;
                        return Point(1, 2);
                    }
                    if (dest == Point(1, 5))
                    {
                        //turn = 2;
                        return Point(2, 0);
                    }
                    if (dest == Point(0, 4))
                    {
                        turn = -1;
                        return Point(1, 0);
                    }
                }
                cout << "Unexpected face transition\n";
                return from;
            }
        }

        Point Next3d(const Point& cur, Point& dir)
        {
            Point p = cur + dir;
            char ch = Get(p);
            if (ch != ' ')
                return p;

            int size = Height() == 12 ? 4 : 50;
            int turn;
            Point dest = NextFace(cur, p, turn);
            dest = dest * size;
            dir = dir >> turn;
            Point mod = p % size;
            mod = mod.BoxRotate(turn, size - 1);
            mod += Point(size, size);
            mod = mod % size;
            Point z = dest + mod;
            if (Get(z) == ' ')
                cout << "ERROR\n";
            return z;
        }

        Point Turn(Point dir, int neg)
        {
            if (neg == -1) // 'R')
                return Point(-dir.y, dir.x);
            // neg == -2
            return Point(dir.y, -dir.x);
        }
    };

    size_t Part1()
    {
        Data data;
        Point pos = Point(data.map[0].find('.'), 0);
        Point dir = Point(1, 0);
        for (auto it = data.steps.begin(); it != data.steps.end(); it++)
        {
            if (*it >= 0)
            {
                for (int s = 0; s < *it; s++)
                {
                    Point next = data.Next(pos, dir);
                    char ch = data.Get(next);
                    if (ch == '#')
                        break;
                    pos = next;
                }
            }
            else
                dir = data.Turn(dir, *it);
        }

        size_t code = (pos.y+1) * 1000 + (pos.x + 1) * 4;
        if (dir.x == 1)
            return code;
        if (dir.x == -1)
            return code + 2;
        if (dir.y == 1)
            return code + 1;
        return code + 3;
    }

    size_t Part2()
    {
        Data data;
        Point pos = Point(data.map[0].find('.'), 0);
        Point dir = Point(1, 0);
        for (auto it = data.steps.begin(); it != data.steps.end(); it++)
        {
            if (*it >= 0)
            {
                for (int s = 0; s < *it; s++)
                {
                    Point dNext = dir;
                    Point next = data.Next3d(pos, dNext);
                    char ch = data.Get(next);
                    if (ch == '#')
                        break;
                    pos = next;
                    dir = dNext;
                }
            }
            else
                dir = data.Turn(dir, *it);
        }

        size_t code = (pos.y + 1) * 1000 + (pos.x + 1) * 4;
        if (dir.x == 1)
            return code;
        if (dir.x == -1)
            return code + 2;
        if (dir.y == 1)
            return code + 1;
        return code + 3;
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