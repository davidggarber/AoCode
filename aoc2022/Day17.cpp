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
        Point home;  // bottom-left of shape
        char shape;

        Rock(char shp, int floor)
            : home(3, floor - 4)
            , shape(shp)
        {
            if (shape == '_')
                home.y = 0;
        }

        Rock(const Rock& r)
        {
            home = r.home;
            shape = r.shape;
        }

        const Rock& operator=(const Rock& r)
        {
            home = r.home;
            shape = r.shape;
            return *this;
        }

        int64_t Top() const
        {
            switch (shape)
            {
            case '|':
                return home.y - 3;
            case '*':
                return home.y - 1;
            case '+':
            case 'L':
                return home.y - 2;
            }
            return home.y;
        }

        bool Intersects(const Rect& rect) const
        {
            switch (shape)
            {
            case '_':
                return rect.Intersects(0, 0, 7, 1);
            case '-':
                return rect.Intersects(home.x, home.y, 4, 1);
            case '|':
                return rect.Intersects(home.x, home.y - 3, 1, 4);
            case '*':
                return rect.Intersects(home.x, home.y - 1, 2, 2);
            case '+':
                return rect.Intersects(home.x + 1, home.y - 2, 1, 3)
                    || rect.Intersects(home.x, home.y - 1, 3, 1);
            case 'L':
                return rect.Intersects(home.x, home.y, 3, 1)
                    || rect.Intersects(home.x + 2, home.y - 2, 1, 2);
            }
            return false;
        }

        bool Intersects(const Rock& rock) const
        {
            switch (shape)
            {
            case '_':
                return rock.Intersects(Rect(0, 0, 7, 1));
            case '-':
                return rock.Intersects(Rect(home.x, home.y, 4, 1));
            case '|':
                return rock.Intersects(Rect(home.x, home.y - 3, 1, 4));
            case '*':
                return rock.Intersects(Rect(home.x, home.y - 1, 2, 2));
            case '+':
                return rock.Intersects(Rect(home.x + 1, home.y - 2, 1, 3))
                    || rock.Intersects(Rect(home.x, home.y - 1, 3, 1));
            case 'L':
                return rock.Intersects(Rect(home.x, home.y, 3, 1))
                    || rock.Intersects(Rect(home.x + 2, home.y - 2, 1, 2));
            }
            return false;
        }

        // Only call when chances of intersection are high
        // Always highest at the end
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

        bool IntersectsWall() const
        {
            // Walls are at X = 0 and X = 8
            switch (shape)
            {
            case '-':
                return home.x < 1 || home.x > 4;
            case '|':
                return home.x < 1 || home.x > 7;
            case '*':
                return home.x < 1 || home.x > 6;
            case '+':
                return home.x < 1 || home.x > 5;
            case 'L':
                return home.x < 1 || home.x > 5;
            }
            return false;
        }

        bool Blow(int dx)
        {
            home.x += dx;
            if (IntersectsWall())
            {
                home.x -= dx;
                return false;
            }
            return true;
        }

        static string Replace(string src, int x, string ins)
        {
            return src.substr(0, x) + ins + src.substr(x + ins.size());
        }

        void Paint(vector<string>& raster, int y, bool newest) const
        {
            switch (shape)
            {
            case '-':
                raster[y] = Replace(raster[y], home.x, newest ? "@@@@" : "####");
                break;
            case '|':
                raster[y-3] = Replace(raster[y-3], home.x, newest ? "@" : "#");
                raster[y - 2] = Replace(raster[y - 2], home.x, newest ? "@" : "#");
                raster[y - 1] = Replace(raster[y - 1], home.x, newest ? "@" : "#");
                raster[y] = Replace(raster[y], home.x, newest ? "@" : "#");
                break;
            case '*':
                raster[y - 1] = Replace(raster[y - 1], home.x, newest ? "@@" : "##");
                raster[y] = Replace(raster[y], home.x, newest ? "@@" : "##");
                break;
            case '+':
                raster[y - 2] = Replace(raster[y - 2], home.x + 1, newest ? "@" : "#");
                raster[y - 1] = Replace(raster[y - 1], home.x, newest ? "@@@" : "###");
                raster[y] = Replace(raster[y], home.x + 1, newest ? "@" : "#");
                break;
            case 'L':
                raster[y - 2] = Replace(raster[y - 2], home.x + 2, newest ? "@" : "#");
                raster[y - 1] = Replace(raster[y - 1], home.x + 2, newest ? "@" : "#");
                raster[y] = Replace(raster[y], home.x, newest ? "@@@" : "###");
                break;
            case '_':
                raster[y] = "+-------+";
                break;
            }
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

    void Print(vector<Rock>& tower, int64_t top, int64_t rows)
    {
        vector<string> raster;
        int bottom = top + rows;
        if (bottom > 0)
            bottom = 0;
        for (int i = top; i <= bottom; i++)
        {
            raster.push_back("|.......|");
        }
        bool newest = true;
        for (auto it = tower.end(); it != tower.begin(); )
        {
            it--;
            if (it->home.y > bottom)
                break;
            it->Paint(raster, it->home.y - top, newest);
            newest = false;
        }

        cout << "\n" << (tower.size() - 1) << " rocks\n";
        int row = top;
        for (auto rit = raster.begin(); rit != raster.end(); rit++)
        {
            cout << *rit << " " << (row++) << "\n";
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
                if (rock.Blow(gasX))
                {
                    if (free < 0 && rock.Intersects(tower))
                        rock.home.x -= gasX;
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
            if (rock.Top() < top)
                top = rock.Top();

            //if (r <= 10)
            //    Print(tower, top, 50);
        }
        //Print(tower, top, 50);
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

        size_t firstCycle_rocks = 0;
        int64_t firstCycle_top = 0;
        size_t eachCycle_rocks = 0;
        int64_t eachCycle_top = 0;
        int64_t skiped_levels = 0;

        size_t total_rocks = 1000000000000UL;
        for (size_t r = 0; r < total_rocks; r++)
        {
            Rock rock(data.shape_sequence[ir], top);
            ir = (ir + 1) % 5;
            bool stop = false;
            int64_t free = 3;
            while (!stop)
            {
                int gasX = data.gas_sequence[g % data.gas_sequence.size()] == '>' ? 1 : -1;
                g = (g + 1) % data.gas_sequence.size();
                if (g == 0)
                {
                    Print(tower, top, 30);
                    size_t cur_rocks = tower.size() - 1;
                    if (firstCycle_rocks == 0)
                    {
                        firstCycle_rocks = cur_rocks;
                        firstCycle_top = top;
                    }
                    else if (eachCycle_rocks == 0)
                    {
                        eachCycle_rocks = cur_rocks - firstCycle_rocks;
                        eachCycle_top = top - firstCycle_top;

                        size_t skipCycles = (total_rocks - cur_rocks) / eachCycle_rocks;
                        r += skipCycles * eachCycle_rocks;
                        skiped_levels = skipCycles * eachCycle_top;
                    }
                }
                if (rock.Blow(gasX))
                {
                    if (free < 0 && rock.Intersects(tower))
                        rock.home.x -= gasX;
                }
                rock.home.y++;
                if (--free < 0 && rock.Intersects(tower))
                {
                    rock.home.y--;
                    stop = true;
                    break;
                }
            }

            tower.push_back(rock);
            if (rock.Top() < top)
                top = rock.Top();
        }
        return -(top + skiped_levels);
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