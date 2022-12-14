#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/12
namespace Day12
{
    auto data_file = "Data\\Day12.txt";

    class Cave
    {
    public:
        string name;
        bool big = false;
        vector<Cave*> links;

        Cave(string n)
        {
            name = n;
            big = n[0] <= 'Z';
        }

        Cave()
            : big(false)
        {
        }

        Cave(Cave& c)
        {
            name = c.name;
            big = c.big;
            links = c.links;
        }
    };

    class Data
    {
    public:
        unordered_map<string, Cave> allCaves;

        Cave* Get(string name)
        {
            auto it = allCaves.find(name);
            if (it == allCaves.end())
            {
                Cave c(name);
                allCaves[name] = c;
                return &allCaves[name];
            }
            return &it->second;
        }

        Cave* start;
        Cave* end;

        Data()
        {
            start = Get("start");
            end = Get("end");

            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    size_t i = line.find('-');
                    string a = line.substr(0, i);
                    string b = line.substr(i + 1);
                    Cave* aa = Get(a);
                    Cave* bb = Get(b);
                    aa->links.push_back(bb);
                    bb->links.push_back(aa);
                }
            }
        }

    };

    class Path
    {
    public:
        vector<Cave*> links;

        bool IsEnd()
        {
            return (links.size() > 0 && links.back()->name == "end");
        }
        bool HasRevisit()
        {
            unordered_set<string> smalls;
            for (auto it = links.begin(); it != links.end(); it++)
            {
                if ((*it)->big)
                    continue;
                if (smalls.find((*it)->name) != smalls.end())
                    return true;
                smalls.insert((*it)->name);
            }
            return false;
        }
        bool CanGo(Cave* c)
        {
            if (links.size() > 0 && links.back()->name == "end")
                return false;
            if (c->name == "start")
                return false;
            auto it = find(links.begin(), links.end(), c);
            if (it == links.end())
                return true;
            return (*it)->big;
        }
        bool CanGoBack(Cave* c)
        {
            if (links.size() > 0 && links.back()->name == "end")
                return false;
            if (c->name == "start")
                return false;
            return !HasRevisit();
        }
        bool equals(Path& p)
        {
            if (links.size() != p.links.size())
                return false;
            auto it = links.begin();
            auto pit = p.links.begin();
            while (it != links.end())
            {
                if (*it != *pit)
                    return false;
                it++;
                pit++;
            }
            return true;
        }
        Path Extend(Cave* next)
        {
            Path p;
            for (auto it = links.begin(); it != links.end(); it++)
            {
                p.links.push_back(*it);
            }
            p.links.push_back(next);
            return p;
        }
        vector<Path> Branches(bool part2)
        {
            vector<Path> vec;
            for (auto it = links.back()->links.begin(); it != links.back()->links.end(); it++)
            {
                if (CanGo(*it))
                    vec.push_back(Extend(*it));
                else if (part2 && CanGoBack(*it))
                    vec.push_back(Extend(*it));
            }
            return vec;
        }
    };

    void Process(vector<Path>& partials, vector<Path>& fulls, bool part2)
    {
        while (partials.size() > 0)
        {
            Path p = partials.back();
            partials.pop_back();
            vector<Path> more = p.Branches(part2);
            for (auto it = more.begin(); it != more.end(); it++)
            {
                if (it->IsEnd())
                    fulls.push_back(*it);
                else
                    partials.push_back(*it);
            }
        }
    }

    size_t Part1()
    {
        Data data;
        vector<Path> paths;
        Path p;
        p.links.push_back(data.start);
        paths.push_back(p);
        vector<Path> fulls;
        Process(paths, fulls, false);

        return fulls.size();
    }

    size_t Part2()
    {
        Data data;
        vector<Path> paths;
        Path p;
        p.links.push_back(data.start);
        paths.push_back(p);
        vector<Path> fulls;
        Process(paths, fulls, true);

        return fulls.size();
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(12, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(12, 2);
            cout << Part2() << "\n";
        }
    }
}