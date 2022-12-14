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

// https://adventofcode.com/2022/day/6
namespace Day6
{
    class Data
    {
    public:
        string data;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                //while (!file.eof())
                {
                    file >> data;
                }
            }
        }
    };

    int CountBits(int bits)
    {
        int count = 0;
        while (bits > 0)
        {
            bits &= (bits - 1);
            count++;
        }
        return count;
    }

    size_t Part1()
    {
        Data data;
        int bits = 0;
        for (int i = 0; i < data.data.size(); i++)
        {
            if (i >= 4)
                bits ^= 1 << (data.data[i - 4] - 'a');
            bits ^= 1 << (data.data[i] - 'a');
            if (CountBits(bits) == 4)
                return i + 1;
        }
        return 0;
    }

    size_t Part2()
    {
        Data data;
        int bits = 0;
        for (int i = 0; i < data.data.size(); i++)
        {
            if (i >= 14)
                bits ^= 1 << (data.data[i - 14] - 'a');
            bits ^= 1 << (data.data[i] - 'a');
            if (CountBits(bits) == 14)
                return i + 1;
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