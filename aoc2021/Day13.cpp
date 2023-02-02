#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>
#include "Point.h"

using namespace std;

// https://adventofcode.com/2021/day/13
namespace Day13
{
    auto data_file = "Data\\Day13.txt";

    struct Fold
    {
        bool horz;
        int at;
    };

    class Data
    {
    public:
        vector<Point> points;
        vector<Fold> folds;
        int width, height;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    if (line.size() == 0)
                        break;
                    points.push_back(Point(line));
                }
                while (!file.eof())
                {
                    getline(file, line);
                    Fold fold;
                    fold.horz = line[11] == 'x';
                    fold.at = atoi(line.substr(13).c_str());
                    folds.push_back(fold);
                    if (fold.horz)
                        width = fold.at;
                    else
                        height = fold.at;
                }
            }
        }

    };

    size_t Part1()
    {
        Data data;
        unordered_set<Point> set;
        for (int i = 0; i < data.points.size(); i++)
        {
            Point pt = data.points[i];
            auto it = data.folds.begin();
            if (it->horz && pt.x > it->at)
            {
                pt.x = it->at * 2 - pt.x;
            }
            else if (!it->horz && pt.y > it->at)
            {
                pt.y = it->at * 2 - pt.y;
            }
            set.insert(pt);
        }

        return set.size();
    }

    string Part2()
    {
        Data data;
        unordered_set<Point> set;
        for (int i = 0; i < data.points.size(); i++)
        {
            Point pt = data.points[i];
            for (auto it = data.folds.begin(); it != data.folds.end(); it++)
            {
                if (it->horz && pt.x > it->at)
                {
                    pt.x = it->at * 2 - pt.x;
                }
                else if (!it->horz && pt.y > it->at)
                {
                    pt.y = it->at * 2 - pt.y;
                }
            }
            set.insert(pt);
        }

        string output(data.height * (data.width + 1), '.');
        for (int y = 0; y < data.height; y++)
            output[(y + 1) * (data.width + 1) - 1] = '\n';

        for (auto it = set.begin(); it != set.end(); it++)
        {
            int offset = it->y * (data.width + 1) + it->x;
            output = output.substr(0, offset) + '#' + output.substr(offset + 1);
        }

        return output;
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