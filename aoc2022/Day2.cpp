#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

// https://adventofcode.com/2022/day/2
namespace Day2
{
    class Data
    {
    public:
        vector<int> lines;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    char them = 0, us = 0;
                    file >> them >> us;
                    if (them == 0)
                        break;
                    int t = them - 'A';
                    int u = us - 'X';
                    if (t == u)
                        lines.push_back(3 + u + 1);
                    else if (t - u == 1 || t - u == -2)
                        lines.push_back(0 + u + 1);
                    else
                        lines.push_back(6 + u + 1);
                }
            }
        }

        int round2()
        {
            int sum = 0;
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    char them = 0, us = 0;
                    file >> them >> us;
                    if (them == 0)
                        break;
                    int t = them - 'A';
                    int u = t;
                    if (us == 'X')
                        u = (t + 2) % 3;
                    else if (us == 'Z')
                        u = (t + 1) % 3;
                    if (t == u)
                        sum += 3 + u + 1;
                    else if (t - u == 1 || t - u == -2)
                        sum += 0 + u + 1;
                    else
                        sum += 6 + u + 1;
                }
            }
            return sum;
        }
    };

    size_t Part1()
    {
        Data data;
        int sum = 0;
        for (int i = 0; i < data.lines.size(); i++)
            sum += data.lines[i];
        return sum;
    }

    size_t Part2()
    {
        Data data;
        return data.round2();
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