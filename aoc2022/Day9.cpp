#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

// https://adventofcode.com/2022/day/9
namespace Day9
{
    class Point
    {
    public:
        short x = 0;
        short y = 0;

        void Move(char dir)
        {
            switch (dir)
            {
            case 'L':
                x--;
                break;
            case 'R':
                x++;
                break;
            case 'U':
                y--;
                break;
            case 'D':
                y++;
                break;
            }
        }

        void Follow(const Point& pt)
        {
            int dx = pt.x - x;
            int dy = pt.y - y;
            if (abs(dx) <= 1 && abs(dy) <= 1)
                return;
            x += dx > 0 ? 1 : dx < 0 ? -1 : 0;
            y += dy > 0 ? 1 : dy < 0 ? -1 : 0;
        }

        int Hash()
        {
            return y * 10000 + x;
            //return y << 16 | x;
        }
    };

    class Data
    {
    public:
        unordered_set<int> prevs;

        Data(int length)
        {
            Point* snake = new Point[length];

            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    char dir;
                    int count;
                    file >> dir >> count;
                    for (int i = 0; i < count; i++)
                    {
                        snake[0].Move(dir);
                        for (int i = 1; i < length; i++)
                        {
                            snake[i].Follow(snake[i - 1]);
                        }
                        prevs.insert(snake[length - 1].Hash());
                    }
                    //cout << "After " << dir << " " << count << " tail is at " << tail.x << "," << tail.y << '\n';
                }
            }
        }
    };

    size_t Part1()
    {
        Data data(2);
        return data.prevs.size();
    }

    size_t Part2()
    {
        Data data(10);
        return data.prevs.size();
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