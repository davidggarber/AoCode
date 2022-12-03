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

// https://adventofcode.com/2022/day/3
namespace Day3
{
    class Data
    {
    public:
        vector<string> left;
        vector<string> right;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    file >> line;
                    left.push_back(line.substr(0, line.size() / 2));
                    right.push_back(line.substr(line.size() / 2));
                }
            }
        }
    };

    size_t Part1()
    {
        Data data;
        size_t sum = 0;
        for (int i = 0; i < data.left.size(); i++)
        {
            for (int a = 0; a < data.left[i].size(); a++)
            {
                char ch = data.left[i][a];
                if (data.right[i].find(ch) < data.right[i].size())
                {
                    if (ch >= 'a')
                        sum += (ch - 'a' + 1);
                    else
                        sum += (ch - 'A' + 27);
                    break;
                }
            }
        }
        return sum;
    }

    size_t Part2()
    {
        Data data;
        int sum = 0;
        for (int i = 0; i < data.left.size(); i += 3)
        {
            string a = data.left[i] + data.right[i];
            string b = data.left[i+1] + data.right[i+1];
            string c = data.left[i+2] + data.right[i+2];
            for (int j = 0; j < a.size(); j++)
            {
                char ch = a[j];
                if (b.find(ch) < b.size() && c.find(ch) < c.size())
                {
                    if (ch >= 'a')
                        sum += (ch - 'a' + 1);
                    else
                        sum += (ch - 'A' + 27);
                    break;
                }

            }
        }
        return sum;
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