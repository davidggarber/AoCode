#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/9
namespace Day9
{
    auto data_file = "Data\\Day9.txt";

    class Data
    {
    public:
        vector<string> map;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    file >> line;
                    map.push_back(line);
                }
            }
        }

        int rows()
        {
            return (int)map.size();
        }

        int cols()
        {
            return (int)map[0].size();
        }

        int depth(int row, int col)
        {
            if (row < 0 || col < 0 || row >= map.size() || col >= map[row].size())
                return 11;
            return 1 + (map[row][col] - '0');
        }

        int fill(int row, int col)
        {
            if (depth(row, col) >= 10)
                return 0;
            int size = 1;
            map[row][col] = '9';
            size += fill(row - 1, col);
            size += fill(row + 1, col);
            size += fill(row, col - 1);
            size += fill(row, col + 1);
            return size;
        }

    };

    int Part1()
    {
        Data data;
        int risk = 0;
        for (int r = 0; r < data.rows(); r++)
        {
            for (int c = 0; c < data.cols(); c++)
            {
                int h = data.depth(r, c);
                if (h < data.depth(r + 1, c) &&
                    h < data.depth(r - 1, c) &&
                    h < data.depth(r, c + 1) &&
                    h < data.depth(r, c - 1))
                {
                    risk += h;
                }
            }
        }
        return risk;
    }

    size_t Part2()
    {
        Data data;
        vector<size_t> basins;
        for (int r = 0; r < data.rows(); r++)
        {
            for (int c = 0; c < data.cols(); c++)
            {
                int f = data.fill(r, c);
                if (f > 0)
                    basins.push_back(f);
            }
        }
        sort(basins.begin(), basins.end());
        reverse(basins.begin(), basins.end());
        return basins[0] * basins[1] * basins[2];
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