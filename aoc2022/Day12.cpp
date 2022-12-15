#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Point.h"

using namespace std;

// https://adventofcode.com/2022/day/12
namespace Day12
{
    class Data
    {
    public:
        vector<string> map;
        int width, height;
        Point start, end;
        unordered_map<Point, int> distances;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);
                    int s = line.find('S');
                    if (s < line.size())
                        start = Point(map.size(), s);
                    int e = line.find('E');
                    if (e < line.size())
                        end = Point(map.size(), e);
                    map.push_back(line);
                }
            }
            height = map.size();
            width = map[0].size();
        }

        int HeightAt(int r, int c)
        {
            if (r == start.y && c == start.x)
                return 0;
            if (r == end.y && c == end.x)
                return 25;
            if (r >= 0 && r < height && c >= 0 && c < width)
                return map[r][c] - 'a';
            return 1000;  // unreachable
        }

        int HeightAt(const Point& pt)
        {
            return HeightAt(pt.y, pt.x);
        }

        bool SetDistance(const Point& pt, int dist, int minHeight)
        {
            if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
                return false;
            int height = HeightAt(pt);
            if (height < minHeight)
                return false;
            auto it = distances.find(pt);
            if (it != distances.end() && it->second <= dist)
                return false;
            distances[pt] = dist;
            return true;
        }

        void CalcDistances()
        {
            SetDistance(end, 0, 25);

            unordered_map<Point, Point> travel;
            travel[Point(end.y - 1, end.x)] = end;
            travel[Point(end.y + 1, end.x)] = end;
            travel[Point(end.y, end.x - 1)] = end;
            travel[Point(end.y, end.x + 1)] = end;

            while (travel.size() > 0)
            {
                auto it = travel.begin();
                Point pt = it->first;
                int minHeight = HeightAt(it->second) - 1;
                int dist = distances[it->second] + 1;
                if (SetDistance(it->first, dist, minHeight))
                {
                    travel[Point(pt.y - 1, pt.x)] = pt;
                    travel[Point(pt.y + 1, pt.x)] = pt;
                    travel[Point(pt.y, pt.x - 1)] = pt;
                    travel[Point(pt.y, pt.x + 1)] = pt;
                }
            }
        }
    };

    size_t Part1()
    {
        Data data;
        data.CalcDistances();
        return data.distances[data.start];
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