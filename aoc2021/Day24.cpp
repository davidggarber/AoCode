#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/24
namespace Day24
{
    auto data_file = "Data\\Day24.txt";

    class Data
    {
    public:
        vector<string> doc;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    doc.push_back(line);
                }
            }
        }

    };

    int Part1()
    {
        Data data;
        return 0;
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
            Stats timer(9, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(9, 2);
            cout << Part2() << "\n";
        }
    }
}