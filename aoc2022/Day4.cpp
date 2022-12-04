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

// https://adventofcode.com/2022/day/4
namespace Day4
{
    class Data
    {
    public:
        int contain, overlap;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            contain = 0;
            overlap = 0;
            if (file.is_open())
            {
                while (!file.eof())
                {
                    int a, b, c, d;
                    char comma, dash;
                    file >> a >> dash >> b >> comma >> c >> dash >> d;
                    if ((a >= c && b <= d) || (c >= a && d <= b))
                        contain++;
                    if (a <= d && b >= c)
                        overlap++;
                }
            }
        }
    };

    size_t Part1()
    {
        Data data;
        return data.contain;
    }

    size_t Part2()
    {
        Data data;
        return data.overlap;
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