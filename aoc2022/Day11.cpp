#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "BigInt.h"

using namespace std;

// https://adventofcode.com/2022/day/11
namespace Day11
{

    class Monkey
    {
    public:
        vector<BigInt> items;
        char op;
        int operand;
        int divisible;
        int if_true, if_false;
        size_t count = 0;

        bool Inspect(BigInt& worry, int& dest, bool div3)
        {
            if (items.size() == 0)
                return false;
            count++;
            worry = items[0];
            items.erase(items.begin());

            BigInt z(operand);
            if (operand == 0)
                z = worry;

            if (op == '+')
                worry += z;
            else if (op == '*')
                worry *= z;
            else
                cout << "Unknown operand: " << op << "\n";

            if (div3)
                worry /= BigInt(3);

            dest = (worry % BigInt(divisible)).IsZero() ? if_true : if_false;
            return true;
        }
    };

    class Data
    {
    public:
        vector<Monkey> monkeys;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);  // Money X
                    Monkey m;

                    getline(file, line);  // Starting items: ...
                    line = line.substr(18);
                    stringstream ss(line);
                    int val;
                    char comma;
                    while (ss >> val)
                    {
                        m.items.push_back(val);
                        ss >> comma;
                    }

                    getline(file, line);  // Operation: new = old ...
                    m.op = line[23];
                    line = line.substr(25);
                    if (line == "old")
                        m.operand = 0;
                    else
                        m.operand = atoi(line.c_str());

                    getline(file, line);  // Test: divisible by ...
                    line = line.substr(21);
                    m.divisible = atoi(line.c_str());

                    getline(file, line);  // If true: throw to monkey Y
                    line = line.substr(29);
                    m.if_true = atoi(line.c_str());

                    getline(file, line);  // If false: throw to monkey Z
                    line = line.substr(30);
                    m.if_false = atoi(line.c_str());

                    if (!file.eof())
                        getline(file, line);  // blank line

                    monkeys.push_back(std::move(m));
                }
            }
        }

        void Round(bool div3 = true)
        {
            for (int i = 0; i < monkeys.size(); i++)
            {
                BigInt item;
                int dest;
                while (monkeys[i].Inspect(item, dest, div3))
                {
                    monkeys[dest].items.push_back(item);
                }
            }
        }
    };

    size_t Part1()
    {
        Data data;
        for (int i = 0; i < 20; i++)
        {
            data.Round();
        }
        vector<size_t> counts;
        for (auto it = data.monkeys.begin(); it != data.monkeys.end(); it++)
        {
            counts.push_back(it->count);
        }
        sort(counts.begin(), counts.end());
        return counts[counts.size() - 1] * counts[counts.size() - 2];
    }

    size_t Part2()
    {
        Data data;
        vector<size_t> counts;
        for (int j = 0; j < 10; j++)
        {
            for (int i = 0; i < 1000; i++)
            {
                data.Round(false);
            }
            counts.clear();
            for (auto it = data.monkeys.begin(); it != data.monkeys.end(); it++)
            {
                counts.push_back(it->count);
            }
            sort(counts.begin(), counts.end());
        }
        return counts[counts.size() - 1] * counts[counts.size() - 2];
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