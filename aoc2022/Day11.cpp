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

// https://adventofcode.com/2022/day/11
namespace Day11
{
    class Monkey
    {
    public:
        vector<int> items;
        char op = '?';
        int operand = 0;
        int divisible = 0;
        int if_true = -1, if_false = -1;
        size_t count = 0;

        bool Inspect(int& worry, int& dest, bool div3)
        {
            if (items.size() == 0)
                return false;
            count++;
            worry = items[0];
            items.erase(items.begin());

            dest = Eval(worry, div3);
            return true;
        }

        int Eval(int& worry, bool div3)
        {
            int z(operand);
            if (operand == 0)
                z = worry;

            if (op == '+')
                worry += z;
            else if (op == '*')
                worry *= z;
            else
                cout << "Unknown operand: " << op << "\n";

            if (div3)
                worry /= int(3);

            return (worry % divisible) == 0 ? if_true : if_false;
        }
    };

    class Data
    {
    public:
        vector<Monkey> monkeys;
        size_t* action_count;

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

            action_count = new size_t[monkeys.size()];
            memset(action_count, 0, monkeys.size() * sizeof(size_t));
        }

        void Round(bool div3 = true)
        {
            for (int i = 0; i < monkeys.size(); i++)
            {
                int item;
                int dest;
                while (monkeys[i].Inspect(item, dest, div3))
                {
                    monkeys[dest].items.push_back(item);
                }
            }
        }

        int Follow(int value, int monkey, bool div3, int rounds)
        {
            int steps = 0;
            int cur = monkey;
            while (rounds > 0)
            {
                action_count[cur]++;
                int dest = monkeys[cur].Eval(value, div3);
                if (dest < cur)
                {
                    rounds--;
                }
                cur = dest;
                steps++;
            }
            return steps;
        }
    };

    size_t Part1()
    {
        Data data;
        for (int m = 0; m < data.monkeys.size(); m++)
        {
            for (auto it = data.monkeys[m].items.begin(); it != data.monkeys[m].items.end(); it++)
            {
                int steps = data.Follow(*it, m, true, 20);
            }
        }
        //for (int i = 0; i < 20; i++)
        //{
        //    data.Round();
        //}
        //vector<size_t> counts;
        //for (auto it = data.monkeys.begin(); it != data.monkeys.end(); it++)
        //{
        //    counts.push_back(it->count);
        //}
        size_t* counts = data.action_count;
        int size = data.monkeys.size();
        sort(counts, counts + size);
        //cout << counts[size - 1] << " * " << counts[size - 2] << " == ";
        return counts[size - 1] * counts[size - 2];
    }

    size_t Part2a()
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

    class ModPrime
    {
    public:
        // We don't care about the actual number. Just the mods relative to a few bases.
        unordered_map<int, int> mods;
        // The bases are all known in advance
        static vector<int> bases;

        ModPrime()
        {
            for (auto it = bases.begin(); it != bases.end(); it++)
            {
                int base = *it;
                mods[base] = 0;
            }
        }

        ModPrime(int value)
        {
            for (auto it = bases.begin(); it != bases.end(); it++)
            {
                int base = *it;
                mods[base] = value % base;
            }
        }

        const ModPrime& operator+=(ModPrime& mp)
        {
            // Adding two numbers adds their mods
            for (auto it = mods.begin(); it != mods.end(); it++)
            {
                it->second = (it->second + mp.mods[it->first]) % it->first;
            }
            return *this;
        }

        const ModPrime& operator*=(ModPrime& mp)
        {
            // Multiplying two numbers multiplies their mods
            for (auto it = mods.begin(); it != mods.end(); it++)
            {
                it->second = (it->second * mp.mods[it->first]) % it->first;
            }
            return *this;
        }

        int operator%(int m)
        {
            // Modding two numbers returns the appropriate mod
            return mods[m];
        }
    };

    /*static*/vector<int> ModPrime::bases;

    class Monkey2
    {
    public:
        vector<int> items;
        char op;
        int raw_operand;
        ModPrime operand;
        int divisible;
        int if_true, if_false;

        int Eval(ModPrime& worry)
        {
            if (op == '+')
                worry += operand;
            else if (op == '*')
                worry *= operand;
            else if (op == '^')  // square
                worry *= worry;
            else
                cout << "Unknown operand: " << op << "\n";

            return (worry % divisible) == 0 ? if_true : if_false;
        }
    };

    class Data2
    {
    public:
        vector<Monkey2> monkeys;
        size_t* action_count;

        Data2()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);  // Money X
                    Monkey2 m;

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
                    {
                        m.raw_operand = 0;
                        m.op = '^';
                    }
                    else
                        m.raw_operand = atoi(line.c_str());

                    getline(file, line);  // Test: divisible by ...
                    line = line.substr(21);
                    m.divisible = atoi(line.c_str());
                    ModPrime::bases.push_back(m.divisible);

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

            // 2nd pass: initialize operands
            for (auto mit = monkeys.begin(); mit != monkeys.end(); mit++)
            {
                mit->operand = ModPrime(mit->raw_operand);
            }

            action_count = new size_t[monkeys.size()];
            memset(action_count, 0, monkeys.size() * sizeof(size_t));
        }

        int Follow(int value, int monkey, int rounds)
        {
            int steps = 0;
            int cur = monkey;
            ModPrime mp(value);
            while (rounds > 0)
            {
                action_count[cur]++;
                int dest = monkeys[cur].Eval(mp);
                if (dest < cur)
                {
                    rounds--;
                }
                cur = dest;
                steps++;
            }
            return steps;
        }
    };


    size_t Part2()
    {
        Data2 data;
        for (int m = 0; m < data.monkeys.size(); m++)
        {
            for (auto it = data.monkeys[m].items.begin(); it != data.monkeys[m].items.end(); it++)
            {
                int steps = data.Follow(*it, m, 10000);
            }
        }
        size_t* counts = data.action_count;
        int size = data.monkeys.size();
        sort(counts, counts + size);
        //cout << counts[size - 1] << " * " << counts[size - 2] << " == ";
        return counts[size - 1] * counts[size - 2];
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