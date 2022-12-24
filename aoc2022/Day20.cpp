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

// https://adventofcode.com/2022/day/20
namespace Day20
{
    class Data
    {
    public:
        vector<int> list;
        vector<int> order;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    int n;
                    file >> n;
                    list.push_back(n);
                    order.push_back(n);
                }
            }
        }
    };

    int Part1()
    {
        Data data;
        int size = data.order.size();
        for (auto it = data.order.begin(); it != data.order.end(); it++)
        {
            auto lit = find(data.list.begin(), data.list.end(), *it);

            int pos = lit - data.list.begin();
            data.list.erase(lit);
            pos += *it;
            while (pos < 0)
                pos += data.list.size();
            pos %= data.list.size();
            lit = data.list.begin() + pos;
            data.list.insert(lit, *it);
        }

        int zPos = find(data.list.begin(), data.list.end(), 0) - data.list.begin();
        int a = (zPos + 1000) % size;
        int b = (zPos + 2000) % size;
        int c = (zPos + 3000) % size;

        int aa = data.list[a];
        int bb = data.list[b];
        int cc = data.list[c];

        return aa + bb + cc;
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
            Stats timer(DayFromNamespace(__FUNCSIG__), 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(DayFromNamespace(__FUNCSIG__), 2);
            cout << Part2() << "\n";
        }
    }
}