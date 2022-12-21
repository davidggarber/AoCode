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
    class Step
    {
    public:
        Point from, to;
        Step(Point f, Point t)
        {
            from = f;
            to = t;
        }

        Step(const Step& s) = default;
        const Step& operator=(const Step& s)
        {
            from = s.from;
            to = s.to;
            return *this;
        }
    };

    class Data
    {
    public:
        vector<string> map;
        int width, height;
        Point start, end;
        unordered_map<Point, int> distances;
        int a_distance;

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
            a_distance = height * width;
        }

        int HeightAt(int r, int c)
        {
            if (r == start.y && c == start.x)
                return 0;
            if (r == end.y && c == end.x)
                return 25;
            if (r >= 0 && r < height && c >= 0 && c < width)
                return map[r][c] - 'a';
            return -1000;  // unreachable
        }

        int HeightAt(const Point& pt)
        {
            return HeightAt(pt.y, pt.x);
        }

        bool SetDistance(const Point& pt, int dist, int minHeight)
        {
            //if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            //    return false;
            int height = HeightAt(pt);
            if (height < minHeight)
                return false;
            auto it = distances.find(pt);
            if (it != distances.end() && it->second <= dist)
                return false;
            distances[pt] = dist;
            if (height == 0 && dist < a_distance)
                a_distance = dist;
            return true;
        }

        void CalcDistances()
        {
            SetDistance(end, 0, 25);

            vector<Step> stack;
            stack.push_back(Step(Point(end.y - 1, end.x), end));
            stack.push_back(Step(Point(end.y + 1, end.x), end));
            stack.push_back(Step(Point(end.y, end.x - 1), end));
            stack.push_back(Step(Point(end.y, end.x + 1), end));

            while (stack.size() > 0)
            {
                Step step = stack.back();
                stack.pop_back();
                Point pt = step.from;
                int minHeight = HeightAt(step.to) - 1;
                int dist = distances[step.to] + 1;
                if (SetDistance(pt, dist, minHeight))
                {
                    stack.push_back(Step(Point(pt.y - 1, pt.x), pt));
                    stack.push_back(Step(Point(pt.y + 1, pt.x), pt));
                    stack.push_back(Step(Point(pt.y, pt.x - 1), pt));
                    stack.push_back(Step(Point(pt.y, pt.x + 1), pt));
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
        data.CalcDistances();
        return data.a_distance;
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