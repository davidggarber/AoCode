#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "Point3d.h";
#include "Box3d.h";

using namespace std;

// https://adventofcode.com/2022/day/18
namespace Day18
{
    class Data
    {
    public:
        unordered_set<Point3d> blocks;
        unordered_set<Point3d> exterior;
        Box3d space;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    int x = 0, y = 0, z = 0;
                    char comma;
                    file >> x >> comma >> y >> comma >> z;
                    Point3d pt(x, y, z);
                    blocks.insert(pt);
                    space |= pt;
                }
            }
        }

        bool Exists(const Point3d pt) const
        {
            return blocks.find(pt) != blocks.end();
        }

        int CountExposed(Point3d dir)
        {
            int count = 0;
            for (auto it = blocks.begin(); it != blocks.end(); it++)
            {
                Point3d adj = *it + dir;
                if (!Exists(adj))
                    count++;
            }
            return count;
        }

        int CountExterior(Point3d dir)
        {
            int count = 0;
            for (auto it = blocks.begin(); it != blocks.end(); it++)
            {
                Point3d adj = *it + dir;
                if (exterior.find(adj) != exterior.end())
                    count++;
            }
            return count;
        }

        bool Filled(const Point3d pt) const
        {
            return exterior.find(pt) != exterior.end();
        }
        bool Fillable(const Point3d pt)
        {
            if (!space.Contains(pt))
                return false;  // Treat exterior of space as already filled
            if (Exists(pt))
                return false;
            return exterior.find(pt) == exterior.end();
        }

        void FillSpace()
        {
            space.Expand(Point3d(1, 1, 1));
            vector<Point3d> stack;
            stack.push_back(Point3d(space.left, space.top, space.front));
            while (stack.size() > 0)
            {
                Point3d p = stack.back();
                stack.pop_back();
                if (Fillable(p))
                {
                    exterior.insert(p);
                    stack.push_back(p + Point3d::WEST);
                    stack.push_back(p + Point3d::EAST);
                    stack.push_back(p + Point3d::NORTH);
                    stack.push_back(p + Point3d::SOUTH);
                    stack.push_back(p + Point3d::UP);
                    stack.push_back(p + Point3d::DOWN);
                }
            }
        }

        void PrintLayer(int z, bool filled) const
        {
            cout << "\nLayer " << z << "\n";
            for (int y = space.top; y < space.Bottom(); y++)
            {
                for (int x = space.left; x < space.Right(); x++)
                {
                    bool ex = Exists(Point3d(x, y, z));
                    bool f = filled && Filled(Point3d(x, y, z));
                    cout << (ex ? "#" : f ? "~" : ".");
                }
                cout << "\n";
            }
        }

        void Print(bool filled) const
        {
            for (int z = space.front; z < space.Back(); z++)
            {
                PrintLayer(z, filled);
            }
            cout << "\n";
        }
    };

    size_t Part1()
    {
        Data data;
        return data.CountExposed(Point3d::WEST)
            + data.CountExposed(Point3d::EAST)
            + data.CountExposed(Point3d::NORTH)
            + data.CountExposed(Point3d::SOUTH)
            + data.CountExposed(Point3d::UP)
            + data.CountExposed(Point3d::DOWN);
    }

    size_t Part2()
    {
        Data data;
        data.FillSpace();
        //data.Print(true);
        return data.CountExterior(Point3d::WEST)
            + data.CountExterior(Point3d::EAST)
            + data.CountExterior(Point3d::NORTH)
            + data.CountExterior(Point3d::SOUTH)
            + data.CountExterior(Point3d::UP)
            + data.CountExterior(Point3d::DOWN);
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