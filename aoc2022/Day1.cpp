#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2022/day/1
namespace Day1
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
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    if (line.empty())
                        lines.push_back(0);
                    else
                    {
                        int d = atoi(line.c_str());
                        lines.push_back(d);
                    }
                }
            }
            lines.push_back(0);
        }

        int most()
        {
            int most = 0;
            int sum = 0;
            for (size_t i = 0; i < lines.size(); i++)
            {
                sum += lines[i];
                if (lines[i] == 0)
                {
                    if (most < sum)
                        most = sum;
                    sum = 0;
                }
            }
            return most;
        }

        int top3()
        {
            vector<int> elves;
            int sum = 0;
            for (size_t i = 0; i < lines.size(); i++)
            {
                sum += lines[i];
                if (lines[i] == 0)
                {
                    elves.push_back(sum);
                    sum = 0;
                }
            }
            sort(elves.begin(), elves.end());
            size_t len = elves.size();
            return elves[len - 1] + elves[len - 2] + elves[len - 3];
        }
    };

    int Part1()
    {
        Data data;
        return data.most();
    }

    size_t Part2()
    {
        Data data;
        return data.top3();
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