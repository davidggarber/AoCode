#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Stopwatch.h"

// https://adventofcode.com/2021/day/2
namespace Day2
{
    auto data_file = "Data\\Day2.txt";

    struct Step
    {
        std::string direction;
        int count;
    };

    class Data
    {
    public:
        std::vector<Step> lines;

        Data()
        {
            std::ifstream file(data_file);
            if (file.is_open())
            {
                while (!file.eof())
                {
                    Step step;
                    file >> step.direction >> step.count;
                    lines.push_back(step);
                }
            }
        }
    };

    int Part1()
    {
        Data data;
        int x = 0, y = 0;
        for (int i = 0; i < data.lines.size(); i++)
        {
            if (data.lines[i].direction == "down")
                y += data.lines[i].count;
            else if (data.lines[i].direction == "up")
                y = std::max(0, y - data.lines[i].count);
            else
                x += data.lines[i].count;
        }

        return x * y;
    }

    int Part2()
    {
        Data data;
        int x = 0, dir = 0, y = 0;
        for (int i = 0; i < data.lines.size(); i++)
        {
            if (data.lines[i].direction == "down")
                dir += data.lines[i].count;
            else if (data.lines[i].direction == "up")
                dir -= data.lines[i].count;
            else
            {
                x += data.lines[i].count;
                y = std::max(0, y + dir * data.lines[i].count);
            }
        }

        return x * y;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(1, 1);
            std::cout << Part1() << "\n";
        }
        {
            Stats timer(1, 2);
            std::cout << Part2() << "\n";
        }
    }
}