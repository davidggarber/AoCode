#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/11
namespace Day11
{
    auto data_file = "Data\\Day11.txt";

    class Data
    {
    public:
        vector<vector<int>> cave;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    vector<int> row;
                    for (int i = 0; i < line.size(); i++)
                    {
                        int n = line[i] - '0';
                        row.push_back(n);
                    }
                    cave.push_back(row);
                }
            }
        }

        void Increment(int r, int c)
        {
            if (r < 0 || c < 0 || r >= cave.size() || c >= cave[r].size())
                return;
            cave[r][c] += 1;
            if (cave[r][c] == 10)
            {
                Increment(r - 1, c-1);
                Increment(r - 1, c);
                Increment(r - 1, c+1);
                Increment(r, c-1);
                Increment(r, c+1);
                Increment(r + 1, c-1);
                Increment(r + 1, c);
                Increment(r + 1, c+1);
            }
        }

        bool Flash(int r, int c)
        {
            if (cave[r][c] >= 10)
            {
                cave[r][c] = 0;
                return true;
            }
            return false;
        }
    };

    int Part1()
    {
        Data data;
        int flashes = 0;
        for (int round = 0; round < 100; round++)
        {
            for (int r = 0; r < data.cave.size(); r++)
            {
                for (int c = 0; c < data.cave[0].size(); c++)
                {
                    data.Increment(r, c);
                }
            }
            for (int r = 0; r < data.cave.size(); r++)
            {
                for (int c = 0; c < data.cave[0].size(); c++)
                {
                    if (data.Flash(r, c))
                        flashes++;
                }
            }
        }
        return flashes;
    }

    size_t Part2()
    {
        Data data;
        int flashes = 0;
        int step = 0;
        do
        {
            step++;  // first step is #1
            flashes = 0;
            for (int r = 0; r < data.cave.size(); r++)
            {
                for (int c = 0; c < data.cave[0].size(); c++)
                {
                    data.Increment(r, c);
                }
            }
            for (int r = 0; r < data.cave.size(); r++)
            {
                for (int c = 0; c < data.cave[0].size(); c++)
                {
                    if (data.Flash(r, c))
                        flashes++;
                }
            }
        } while (flashes != data.cave.size() * data.cave[0].size());
        return step;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(11, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(11, 2);
            cout << Part2() << "\n";
        }
    }
}