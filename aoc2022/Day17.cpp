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
                home = Point(1, floor);
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
                    if (home + *it == rock.home + *rit)
                        return true;
                }
            }
            return false;
        }

        bool Intersects(vector<Rock>& tower)
        {
            for (auto it = tower.end(); it != tower.begin(); )
            {
                it--;
                if (Intersects(*it))
                    return true;
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

    void Print(vector<Rock>& tower, int top)
    {
        vector<string> raster;
        for (int i = top; i <= 0; i++)
        {
            raster.push_back("|.......|");
        }
        for (auto it = tower.begin(); it != tower.end(); it++)
        {
            for (auto oit = it->offsets.begin(); oit != it->offsets.end(); oit++)
            {
                Point pt = it->home + *oit;
                string s = raster[pt.y - top];
                s = s.substr(0, pt.x) + '#' + s.substr(pt.x + 1);
                raster[pt.y - top] = s;
            }
        }
        raster.pop_back();
        raster.push_back("+-------+");
        for (auto rit = raster.begin(); rit != raster.end(); rit++)
        {
            cout << *rit << "\n";
        }
    }

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
            int free = 3;
            while (!stop)
            {
                int gasX = data.gas_sequence[g % data.gas_sequence.size()] == '>' ? 1 : -1;
                g++;
                if (rock.Blow(gasX, 0, 8))
                {
                    if (rock.Intersects(tower))
                        rock.Blow(-gasX, 0, 8);
                }
                rock.home.y++;
                if (--free < 0 &&  rock.Intersects(tower))
                {
                    rock.home.y--;
                    stop = true;
                    break;
                }
            }
            tower.push_back(rock);
            if (rock.home.y < top)
                top = rock.home.y;
            //Print(tower, top);
        }
        return -top;
    }

    int64_t Part2()
    {
        Data data;
        vector<Rock> tower;
        tower.push_back(Rock('_', 0));

        int64_t top = 0;
        int ir = 0;
        int g = 0;

        // IDEA: every time ir == 0 && g == 0, make a scan of the top of the tower.
        // Cache that. Then when the next full loop is done, associate before -> after, plus height increase
        // Any time we see that pattern again, we can fast forward

        for (size_t r = 0; r < 2022; r++)  // 1000000000000UL
        {
            Rock rock(data.shape_sequence[ir], top);
            ir = (ir + 1) % 5;
            bool stop = false;
            while (!stop)
            {
                int gasX = data.gas_sequence[g % data.gas_sequence.size()] == '>' ? 1 : -1;
                g = (g + 1) % data.gas_sequence.size();
                if (rock.Blow(gasX, 0, 8))
                {
                    if (rock.Intersects(tower))
                        rock.Blow(-gasX, 0, 8);
                }
                rock.home.y++;
                if (rock.Intersects(tower))
                {
                    rock.home.y--;
                    stop = true;
                    break;
                }
            }

            // IDEA: insert sorted
            tower.push_back(rock);
            if (rock.home.y < top)
                top = rock.home.y;

            // IDEA: check for completed rows
            // then purge all rocks whose home is below that row

            //Print(tower, top);
        }
        return -top;
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