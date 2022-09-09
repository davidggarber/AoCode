#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Stopwatch.h"

namespace Day1c
{
    auto data_file = "Data\\Day1.txt";

    class Data
    {
    public:
        std::vector<int> depths;

        Data()
        {
            std::ifstream file(data_file);
            if (file.is_open())
            {
                while (!file.eof())
                {
                    int depth;
                    file >> depth;
                    depths.push_back(depth);
                }
            }
        }
    };

    int Part1()
    {
        Data data;
        int count = -1;
        int prev = -1;
        for (int i = 0; i < data.depths.size(); i++)
        {
            if (data.depths[i] > prev)
                count++;
            prev = data.depths[i];
        }

        return count;
    }

    int Part2()
    {
        Data data;

        int window = 0;
        for (int i = 0; i < 3; i++)
        {
            window += data.depths[i];
        }

        int count = 0;
        for (int i = 3; i < data.depths.size(); i++)
        {
            int prev = window;
            window += data.depths[i] - data.depths[i - 3];
            if (window > prev)
                count++;
        }

        return count;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Or just part 1, if it returns false.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(1, 1);
            std::cout << Part1() << "\n";
        }
        {
            Stats timer(1, 2);
            std::cout << Part2() << "\n";
        }
    }
}