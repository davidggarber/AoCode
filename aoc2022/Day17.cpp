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
#include "Rect.h"

using namespace std;

// https://adventofcode.com/2022/day/17
namespace Day17
{
    class Rock
    {
    public:
        Point home;
        Point size;
        vector<Point> offsets;
        
        Rock(char shape, int floor)
            : home(3, floor - 4)
        {
            switch (shape)
            {
            case '-':
                offsets.push_back(Point(0, 0));
                offsets.push_back(Point(1, 0));
                offsets.push_back(Point(2, 0));
                offsets.push_back(Point(3, 0));
                size = Point(4, 1);
                break;
            case '+':
                offsets.push_back(Point(1, 0));
                offsets.push_back(Point(0, 1));
                offsets.push_back(Point(1, 1));
                offsets.push_back(Point(2, 1));
                offsets.push_back(Point(1, 2));
                size = Point(3, 3);
                break;
            case 'L':
                offsets.push_back(Point(2, 0));
                offsets.push_back(Point(2, 1));
                offsets.push_back(Point(2, 2));
                offsets.push_back(Point(0, 2));
                offsets.push_back(Point(1, 2));
                size = Point(3, 3);
                break;
            case '|':
                offsets.push_back(Point(0, 0));
                offsets.push_back(Point(0, 1));
                offsets.push_back(Point(0, 2));
                offsets.push_back(Point(0, 3));
                size = Point(1, 4);
                break;
            case '*':
                offsets.push_back(Point(0, 0));
                offsets.push_back(Point(1, 0));
                offsets.push_back(Point(0, 1));
                offsets.push_back(Point(1, 1));
                size = Point(2, 2);
                break;
            case '_':  // Actual floor
                for (int f = 0; f < 7; f++)
                    offsets.push_back(Point(f, 0));
                size = Point(7, 1);
                home.y = floor;
                break;
            }
            home.y -= size.y - 1;
        }

        Rock(const Rock& r)
        {
            home = r.home;
            size = r.size;
            offsets = r.offsets;
        }

        const Rock& operator=(const Rock& r)
        {
            home = r.home;
            size = r.size;
            offsets = r.offsets;
            return *this;
        }

        Rect Bounding() const
        {
            //Point far = home;
            //for (auto it = offsets.begin(); it != offsets.end(); it++)
            //{
            //    far.x = max(far.x, it->x);
            //    far.y = max(far.y, it->y);
            //}
            return Rect(home, home + size);
        }
        
        bool Intersects(const Rock& rock)
        {
            if ((Bounding() & rock.Bounding()).IsEmpty())
                return false;
            for (auto it = offsets.begin(); it != offsets.end(); it++)
            {
                for (auto rit = rock.offsets.begin(); rit != rock.offsets.end(); rit++)
                {
                    if (*it == *rit)
                        return true;
                }
            }
            return false;
        }

        bool Blow(int dx, int64_t leftWall, int64_t rightWall)
        {
            Rect b = Bounding();
            if (b.left + dx > leftWall && b.Right() + dx <= rightWall)
            {
                home.x += dx;
                return true;
            }
            return false;
        }
    };

    class Data
    {
    public:
        string shape_sequence;
        string gas_sequence;

        Data()
        {
            shape_sequence = "-+L|*";

            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                getline(file, gas_sequence);
            }
        }
    };

    size_t Part1()
    {
        Data data;
        vector<Rock> tower;
        tower.push_back(Rock('_', 0));

        int top = 0;
        int g = 0;
        for (int r = 0; r < 2022; r++)
        {
            Rock rock(data.shape_sequence[r % 5], top);
            bool stop = false;
            while (!stop)
            {
                int gasX = data.gas_sequence[g % data.gas_sequence.size()] == '>' ? 1 : -1;
                g++;
                rock.Blow(gasX, 0, 8);
                rock.home.y++;
                for (auto it = tower.end(); it != tower.begin(); )
                {
                    it--;
                    if (rock.Intersects(*it))
                    {
                        rock.home.y--;
                        stop = true;
                        break;
                    }
                }
            }
            tower.push_back(rock);
            if (rock.home.y < top)
                top = rock.home.y;
        }
        return -top;
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