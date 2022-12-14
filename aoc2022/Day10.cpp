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

// https://adventofcode.com/2022/day/10
namespace Day10
{
    class Data
    {
    public:
        vector<int> start;

        Data()
        {
            int x = 1;
            start.push_back(0);  // cycle 0 starts with no x, and ends initializing x=1
            start.push_back(x);  // cycle 1 starts with x=1

            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string cmd;
                    int val;
                    file >> cmd;
                    if (cmd == "noop")
                        start.push_back(x);
                    else
                    {
                        file >> val;
                        // no change in first cycle
                        start.push_back(x);  // start of 2nd cycle
                        x += val;  // x changes at end of that cycle
                        start.push_back(x);  // start of next cmd
                    }
                }
                string line;
            }
        }
    };

    size_t Part1()
    {
        Data data;
        size_t sum = 0;
        for (int i = 20; i < data.start.size(); i += 40)
            sum += data.start[i] * i;
        return sum;
    }

    size_t Part2()
    {
        Data data;
        for (int i = 1; i < data.start.size(); i++)
        {
            int pos = ((i - 1) % 40);
            if (pos == 0)
                cout << "\n";
            int x = data.start[i];
            if (x >= pos - 1 && x <= pos + 1)
                cout << "#";
            else
                cout << ".";
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