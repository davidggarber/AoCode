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

// https://adventofcode.com/2022/day/5
namespace Day5
{
    struct Move
    {
    public:
        int count;
        int from;
        int to;
    };

    class Data
    {
    public:
        vector<Move> lines;
        vector<char> stacks[9];

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                bool data = true;
                while (!file.eof())
                {
                    if (data)
                    {
                        string line;
                        getline(file, line);
                        if (line.size() > 2 && line[1] == '1')
                        {
                            data = false;
                            continue;
                        }
                        for (int i = 1; i < line.size(); i += 4)
                        {
                            char x = line[i];
                            if (x != ' ')
                                stacks[i / 4].insert(stacks[i / 4].begin(), x);
                        }
                    }
                    else
                    {
                        string m, f, t;
                        Move move;
                        file >> m >> move.count >> f >> move.from >> t >> move.to;
                        move.from--;
                        move.to--;
                        lines.push_back(move);
                    }
                }
            }
        }
    };

    string Part1()
    {
        Data data;
        for (int i = 0; i < data.lines.size(); i++)
        {
            Move& move = data.lines[i];
            for (int j = 0; j < move.count; j++)
            {
                char c = data.stacks[move.from][data.stacks[move.from].size() - 1];
                data.stacks[move.from].pop_back();
                data.stacks[move.to].push_back(c);
            }
        }
        string s = "";
        for (int k = 0; k < 9; k++)
        {
            s += data.stacks[k][data.stacks[k].size() - 1];
        }
        return s;
    }

    string Part2()
    {
        Data data;
        for (int i = 0; i < data.lines.size(); i++)
        {
            Move& move = data.lines[i];
            int first = data.stacks[move.from].size() - move.count;
            for (int j = 0; j < move.count; j++)
            {
                char c = data.stacks[move.from][first + j];
                data.stacks[move.to].push_back(c);
            }
            for (int j = 0; j < move.count; j++)
                data.stacks[move.from].pop_back();
        }
        string s = "";
        for (int k = 0; k < 9; k++)
        {
            s += data.stacks[k][data.stacks[k].size() - 1];
        }
        return s;
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