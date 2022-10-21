#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Stopwatch.h"

using namespace std;

// https://adventofcode.com/2021/day/6
namespace Day6
{
    auto data_file = "Data\\Day6.txt";

    class Data
    {
    public:
        size_t pop[7] = { 0 };
        size_t spawn[7] = { 0 };

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                size_t d;
                char c;  // comma
                while (!file.eof())
                {
                    file >> d;
                    pop[d]++;
                    if (!file.eof())
                        file >> c;  // comma
                }
            }
        }

        size_t TotalPopulation()
        {
            size_t t = 0;
            for (size_t i = 0; i < 7; i++)
            {
                t += pop[i] + spawn[i];
            }
            return t;
        }

        void Spawn(int day)
        {
            // Record how many births happened from today's adults
            spawn[(day + 2) % 7] = pop[day % 7];
            // Then graduate any of today's kids to adults
            pop[day % 7] += spawn[day % 7];
            spawn[day % 7] = 0;
        }
    };


    size_t Part1()
    {
        Data data;

        for (int day = 0; day < 80; day++)
        {
            data.Spawn(day);
        }

        return data.TotalPopulation();
    }

    size_t Part2()
    {
        Data data;

        for (int day = 0; day < 256; day++)
        {
            data.Spawn(day);
        }

        return data.TotalPopulation();
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(6, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(6, 2);
            cout << Part2() << "\n";
        }
    }
}