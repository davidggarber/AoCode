#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Point.h"

using namespace std;

// https://adventofcode.com/2022/day/20
namespace Day20
{
    class Data
    {
    public:
        vector<Point> list;

        Data(int64_t key = 1)
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                int64_t order = 0;
                while (!file.eof())
                {
                    int64_t n;
                    file >> n;
                    list.push_back(Point(n * key, order++));
                }
            }
        }

        int64_t FindOrder(int64_t order)
        {
            auto it = find_if(list.begin(), list.end(), [&order](const Point& p) { return p.y == order; });
            return it - list.begin();
        }

        int64_t FindValue(int64_t value)
        {
            auto it = find_if(list.begin(), list.end(), [&value](const Point& p) { return p.x == value; });
            return it - list.begin();
        }

        void Print()
        {
            for (auto it = list.begin(); it != list.end(); it++)
            {
                if (it != list.begin())
                    cout << ", ";
                cout << it->x;
            }
            cout << "\n";
        }

        void Shuffle()
        {
            int64_t gaps = list.size() - 1;
            for (int64_t order = 0; order < list.size(); order++)
            {
                int64_t pos = FindOrder(order);
                Point val = list[pos];
                auto lit = list.begin() + pos;
                list.erase(lit);
                // pos is now the index of the item AFTER the removed item
                pos += val.x;
                pos = (((pos % gaps) + gaps) % gaps);
                lit = list.begin() + pos;
                list.insert(lit, val);
            }
        }
    };

    int Part1()
    {
        Data data;
        int64_t size = data.list.size();
        //data.Print();
        data.Shuffle();
        //data.Print();

        int64_t zPos = data.FindValue(0);
        int64_t a = (zPos + 1000) % size;
        int64_t b = (zPos + 2000) % size;
        int64_t c = (zPos + 3000) % size;

        int64_t aa = data.list[a].x;
        int64_t bb = data.list[b].x;
        int64_t cc = data.list[c].x;

        return aa + bb + cc;
    }

    size_t Part2()
    {
        Data data(811589153);
        int64_t size = data.list.size();
        //data.Print();
        for (int64_t i = 0; i < 10; i++)
        {
            data.Shuffle();
            //data.Print();
        }

        int64_t zPos = data.FindValue(0);
        int64_t a = (zPos + 1000) % size;
        int64_t b = (zPos + 2000) % size;
        int64_t c = (zPos + 3000) % size;

        int64_t aa = data.list[a].x;
        int64_t bb = data.list[b].x;
        int64_t cc = data.list[c].x;

        return aa + bb + cc;
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