#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"

using namespace std;

// https://adventofcode.com/2021/day/7
namespace Day7
{
    auto data_file = "Data\\Day7.txt";

    class Data
    {
    public:
        vector<int> pos;
        unordered_map<int, int> map;
        int sum = 0;
        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                size_t d;
                char c;  // comma
                while (!file.eof())
                {
                    file >> d;
                    sum += (int)d;
                    pos.push_back(d);
                    auto f = map.find(d);
                    if (f == map.end())
                        map.insert({ d, 1 });
                    else
                        f->second++;
                    if (!file.eof())
                        file >> c;  // comma
                }
            }
        }

        int at(int p)
        {
            auto f = map.find(p);
            return f == map.end() ? 0 : f->second;
        }
    };

    double Part1()
    {
        Data data;

        int cost = data.sum;
        int target = 0;  // everyone moves to zero
        int left = 0;    // 0 crabs left of / less-than zero
        int at = data.at(0);  // some crabs at zero
        int right = data.pos.size() - at;  // all the rest greater than

        int min = cost;
        int t = 0;
        while (right > 0)
        {
            left += at;
            at = data.at(++t);
            cost += left - right;
            right -= at;
            if (cost < min)
            {
                min = cost;
                target = t;
            }
            else
                break;
        }

        return min;
    }

    size_t Part2()
    {
        Data data;

        size_t cost = 0;
        for (auto it = data.pos.begin(); it != data.pos.end(); it++)
        {
            cost += *it * (*it + 1) / 2;
        }

        int target = 0;  // initial cost was if everyone moves to zero
        int at = data.at(0);  // some crabs at zero
        int right = data.pos.size() - at;  // all the rest greater than

        size_t min = cost;
        int t = 0;
        while (right > 0)
        {
            at = data.at(++t);
            right -= at;

            cost = 0;
            for (auto it = data.map.begin(); it != data.map.end(); it++)
            {
                int dist = abs(it->first - t);
                cost += (dist * (dist + 1) / 2) * it->second;
            }

            if (cost < min)
            {
                min = cost;
                target = t;
            }
        }

        return min;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(7, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(7, 2);
            cout << Part2() << "\n";
        }
    }
}