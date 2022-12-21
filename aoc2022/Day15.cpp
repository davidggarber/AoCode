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

// https://adventofcode.com/2022/day/15
namespace Day15
{
    class Sensor
    {
    public:
        Point sensor_at;
        Point beacon_at;
        int64_t range;
        bool BreadthAtRow(int row, int64_t& left, int64_t& right)
        {
            int64_t dx = range - abs(sensor_at.y - row);
            if (dx < 0)
                return false;
            left = sensor_at.x - dx;
            right = sensor_at.x + dx;
            return true;
        }

        Sensor(Point s, Point b)
        {
            sensor_at = s;
            beacon_at = b;
            range = s.ManhattenDistance(b);
        }
        Sensor(const Sensor& s)
        {
            sensor_at = s.sensor_at;
            beacon_at = s.beacon_at;
            range = s.range;
        }
        const Sensor& operator=(const Sensor& s)
        {
            sensor_at = s.sensor_at;
            beacon_at = s.beacon_at;
            range = s.range;
            return *this;
        }
    };

    class Data
    {
    public:
        vector<Sensor> sensors;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);

                    stringstream ss(line);
                    Point sens, beac;
                    string sensor, at, closest, beacon, is;
                    char eq, comma, colon, x, y;
                    ss >> sensor >> at >> x >> eq >> sens.x >> comma >> y >> eq >> sens.y;
                    ss >> colon >> closest >> beacon >> is >> at >> x >> eq >> beac.x >> comma >> y >> eq >> beac.y;
                    sensors.push_back(Sensor(sens, beac));
                }
            }
        }

        bool MissingBeacon(int64_t max_coord, Point& missing)
        {
            for (int row = 0; row <= max_coord; row++)
            {
                vector<Point> spans;  // x==left, y=right (inclusive)
                for (auto it = sensors.begin(); it != sensors.end(); it++)
                {
                    int64_t left, right;
                    if (it->BreadthAtRow(row, left, right))
                    {
                        for (auto sit = spans.begin(); true; sit++)
                        {
                            if (sit == spans.end() || right < sit->x - 1)
                            {
                                // new discreet sub-range
                                spans.insert(sit, Point(left, right));
                                break;
                            }
                            if (left <= sit->y)
                            {
                                // overlaps this sub-range
                                sit->x = min(sit->x, left);
                                if (sit->y >= right)
                                    break;
                                // extends this sub-range to the right
                                auto grow = sit;
                                grow->y = right;
                                sit++;
                                while (sit != spans.end())
                                {
                                    if (sit->x > right + 1)
                                        break;
                                    // merges into next sub-range
                                    grow->y = max(grow->y, sit->y);
                                    sit = spans.erase(sit);
                                }
                                break;
                            }
                        }
                    }
                }
                int cannot = 0;
                for (auto sit = spans.begin(); sit != spans.end(); sit++)
                {
                    cannot += 1 + min(max_coord, sit->y) - max((int64_t)0, sit->x);
                }

                if (cannot < max_coord + 1)
                {
                    missing.y = row;
                    missing.x = 0;
                    for (auto sit = spans.begin(); sit != spans.end(); sit++)
                    {
                        if (sit->x > missing.x)
                            break;
                        missing.x = sit->y + 1;
                    }
                    return true;
                }
            }
            return false;
        }
    };

    size_t Part1()
    {
        Data data;
        vector<Point> spans;  // x==left, y=right (inclusive)
        int64_t test_row = 2000000;  // 10
        for (auto it = data.sensors.begin(); it != data.sensors.end(); it++)
        {
            int64_t left, right;
            if (it->BreadthAtRow(test_row, left, right))
            {
                for (auto sit = spans.begin(); true; sit++)
                {
                    if (sit == spans.end() || right < sit->x - 1)
                    {
                        // new discreet sub-range
                        spans.insert(sit, Point(left, right));
                        break;
                    }
                    if (left <= sit->y)
                    {
                        // overlaps this sub-range
                        sit->x = min(sit->x, left);
                        if (sit->y >= right)
                            break;
                        // extends this sub-range to the right
                        auto grow = sit;
                        grow->y = right;
                        sit++;
                        while (sit != spans.end())
                        {
                            if (sit->x > right + 1)
                                break;
                            // merges into next sub-range
                            grow->y = max(grow->y, sit->y);
                            sit = spans.erase(sit);
                        }
                        break;
                    }
                }
            }
        }
        int cannot = 0;
        for (auto sit = spans.begin(); sit != spans.end(); sit++)
        {
            cannot += 1 + sit->y - sit->x;
        }
        unordered_set<int64_t> beacons;
        for (auto it = data.sensors.begin(); it != data.sensors.end(); it++)
        {
            if (it->beacon_at.y == test_row)
                beacons.insert(it->beacon_at.x);
        }
        return cannot - beacons.size();
    }

    size_t Part2()
    {
        Data data;
        Point missing;
        if (!data.MissingBeacon(4000000, missing))  // 20
            return 0;
        return missing.x * 4000000 + missing.y;
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