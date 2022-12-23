#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Point3d.h"

using namespace std;

// https://adventofcode.com/2022/day/19
namespace Day19
{
    enum  // Resources
    {
        Ore = 0,
        Clay,
        Obsidian,
        Geode,
        RESOURCE_COUNT
    };

    class Cost
    {
    public:
        int _[RESOURCE_COUNT];

        Cost()
        {
            memset(_, 0, RESOURCE_COUNT * sizeof(int));
        }

        Cost(const Cost& c)
        {
            memcpy(_, c._, RESOURCE_COUNT * sizeof(int));
        }

        const Cost& operator=(const Cost& c)
        {
            memcpy(_, c._, RESOURCE_COUNT * sizeof(int));
            return *this;
        }

        const Cost& operator+=(const Cost& c)
        {
            for (int i = 0; i < RESOURCE_COUNT; i++)
                _[i] += c._[i];
            return *this;
        }

        const Cost& operator-=(const Cost& c)
        {
            for (int i = 0; i < RESOURCE_COUNT; i++)
                _[i] -= c._[i];
            return *this;
        }

        bool operator>=(const Cost& c) const
        {
            for (int i = 0; i < RESOURCE_COUNT; i++)
            {
                if (_[i] < c._[i])
                    return false;
            }
            return true;
        }
    };

    class Blueprint
    {
    public:
        Cost robot_costs[RESOURCE_COUNT];  // x is ore, y is clay, z is obsidian


        Blueprint()
        {
            memset(robot_costs, 0, RESOURCE_COUNT * sizeof(Cost));
        }

        Blueprint(const Blueprint& b)
        {
            memcpy(robot_costs, b.robot_costs, RESOURCE_COUNT * sizeof(Cost));
        }

        const Blueprint& operator=(const Blueprint& b)
        {
            memcpy(robot_costs, b.robot_costs, RESOURCE_COUNT * sizeof(Cost));
            return *this;
        }
    };

    class Data
    {
    public:
        vector<Blueprint> blueprints;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    Blueprint blue;
                    string bp, each, ore, robot, costs, clay, obsidian, andd, geode;
                    char punc;
                    int index;
                    file >> bp >> index >> punc;
                    file >> each >> ore >> robot >> costs >> blue.robot_costs[Ore]._[Ore] >> ore;
                    file >> each >> clay >> robot >> costs >> blue.robot_costs[Clay]._[Ore] >> ore;
                    file >> each >> obsidian >> robot >> costs >> blue.robot_costs[Obsidian]._[Ore] >> ore >> andd >> blue.robot_costs[Obsidian]._[Clay] >> clay;
                    file >> each >> geode >> robot >> costs >> blue.robot_costs[Geode]._[Ore] >> ore >> andd >> blue.robot_costs[Geode]._[Obsidian] >> obsidian;
                    blueprints.push_back(blue);
                }
            }
        }

        void MaxGeode(int minutes)
        {

        }
    };

    class Status
    {
    public:
        int minute;
        Cost supplies;
        Cost robots;

        Status()
            : minute(0)
        {
            robots._[Ore] = 1;  // Always start with one ore robot and no resources
        }

        Status(const Status& s)
            : minute(s.minute)
            , supplies(s.supplies)
            , robots(s.robots)
        {
        }

        const Status& operator=(const Status& s)
        {
            supplies = s.supplies;
            robots = s.robots;
            return *this;
        }

        bool CanBuildAnything(const Blueprint& b) const
        {
            for (int i = 0; i < RESOURCE_COUNT; i++)
            {
                if (!(supplies >= b.robot_costs[i]))
                    return false;
            }
            return true;
        }

        vector<Status> Options(const Blueprint& b) const
        {
            vector<Status> options;

            // We always gather
            Status gather(*this);
            gather.minute++;
            gather.supplies += robots;

            // If we can build a geode robot, always do that
            if (supplies >= b.robot_costs[Geode])
            {
                Status build(gather);
                build.robots._[Geode]++;
                build.supplies -= b.robot_costs[Geode];
                options.push_back(build);
                return options;
            }
            else if (supplies._[Obsidian] >= b.robot_costs[Geode]._[Obsidian] - robots._[Obsidian])
            {
                // We'll have enough obsidian if we just gather. 
                options.push_back(gather);
            }

            // Else if we can build an obsidian robot, always do that
            if (supplies >= b.robot_costs[Obsidian])
            {
                Status build(gather);
                build.robots._[Obsidian]++;
                build.supplies -= b.robot_costs[Obsidian];
                options.push_back(build);
                return options;
            }
            else if (supplies._[Clay] >= b.robot_costs[Obsidian]._[Clay] - robots._[Clay])
            {
                // We'll have enough clay if we just gather. 
                options.push_back(gather);
            }

            // Or we can both gather and also build something
            for (int i = 0; i <= Clay; i++)
            {
                if (supplies >= b.robot_costs[i])
                {
                    Status build(gather);
                    build.robots._[i]++;
                    build.supplies -= b.robot_costs[i];
                    options.push_back(build);
                }
            }

            if (options.size() < 2)  // If we can build either, we probably should build one
                options.push_back(gather);

            return options;
        }
    };

    int Optimize(const Blueprint& b, int minutes)
    {
        int max_geode = 0;
        int max_obsidian = 0;
        vector<Status> stack;
        stack.push_back(Status());
        while (stack.size() > 0)
        {
            Status s = stack.back();
            stack.pop_back();
            if (s.minute >= minutes)
            {
                if (s.supplies._[Geode] > max_geode)
                {
                    max_geode = s.supplies._[Geode];
                    cout << "Could make " << max_geode << " geodes.\n";
                }
                else if (s.supplies._[Obsidian] > max_obsidian)
                {
                    max_obsidian = s.supplies._[Obsidian];
                }
            }
            else
            {
                bool root = stack.size() == 0;
                vector<Status> options = s.Options(b);

                for (auto it = options.begin(); it != options.end(); it++)
                {
                    stack.push_back(*it);
                }
                //if (root && stack.size() > 0)
                //    cout << "Oldest branch is now at " << stack[0].minute << "\n";
            }
        }
        if (max_geode == 0)
            cout << "Got " << max_obsidian << " toward first geode robot (" << b.robot_costs[Geode]._[Obsidian] << ")\n";

        return max_geode;
    }

    size_t Part1()
    {
        Data data;
        int sum = 0;
        for (int i = 0; i < data.blueprints.size(); i++)
        {
            cout << "\nBlueprint " << (i + 1) << "\n";
            int quality = Optimize(data.blueprints[i], 24);
            sum += quality * (i + 1);
        }
        return sum;
    }

    size_t Part2()
    {
        Data data;
        int product = 1;
        for (int i = 0; i < min(3, (int)data.blueprints.size()); i++)
        {
            cout << "\nBlueprint " << (i + 1) << "\n";
            int quality = Optimize(data.blueprints[i], 32);
            product *= quality;
        }
        return product;
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