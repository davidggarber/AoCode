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

// https://adventofcode.com/2022/day/7
namespace Day7
{
    class File
    {
    public:
        int size;
        string name;
    };

    class Dir
    {
    public:
        string name;
        vector<File> files;
        unordered_map<string, Dir*> dirs;

        int TotalSize()
        {
            int size = 0;
            for (auto fit = files.begin(); fit != files.end(); fit++)
                size += fit->size;
            for (auto dit : dirs)
                size += dit.second->TotalSize();
            return size;
        }
    };

    class Data
    {
    public:
        Dir root;
        vector<Dir*> allDirs;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                vector<Dir*> path;
                while (!file.eof())
                {
                    string line;
                    getline(file, line);

                    if (line[0] == '$')
                    {
                        if (line.substr(2, 2) == "cd")
                        {
                            string targ = line.substr(5);
                            if (targ == "..")
                                path.pop_back();
                            else if (targ == "/")
                            {
                                path.clear();
                                path.push_back(&root);
                            }
                            else
                            {
                                path.push_back(path[path.size() - 1]->dirs[targ]);
                            }
                        }
                        else  // "ls"
                        {

                        }
                    }
                    else  // output of "ls"
                    {
                        if (line.substr(0, 3) == "dir")
                        {
                            Dir* d = new Dir();
                            d->name = line.substr(4);
                            path[path.size() - 1]->dirs[d->name] = d;
                            allDirs.push_back(d);
                        }
                        else {
                            File f;
                            stringstream ss(line);
                            ss >> f.size >> f.name;
                            path[path.size() - 1]->files.push_back(f);
                        }
                    }
                }
            }
        }
    };

    size_t Part1()
    {
        Data data;
        int sum = 0;
        for (auto it = data.allDirs.begin(); it != data.allDirs.end(); it++)
        {
            int ts = (*it)->TotalSize();
            if (ts <= 100000)
                sum += ts;
        }
        return sum;
    }

    size_t Part2()
    {
        Data data;
        size_t diskSize = 70000000;
        size_t maxSize = diskSize - 30000000;
        size_t curSize = data.root.TotalSize();
        size_t minDel = curSize - maxSize;

        size_t bestDel = diskSize;
        for (auto it = data.allDirs.begin(); it != data.allDirs.end(); it++)
        {
            int ts = (*it)->TotalSize();
            if (ts >= minDel && ts < bestDel)
                bestDel = ts;
        }
        return bestDel;
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