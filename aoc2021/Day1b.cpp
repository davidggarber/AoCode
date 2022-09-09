#include <iostream>
#include <fstream>
#include <string>
#include "Stopwatch.h"

namespace Day1b
{
    auto data_file = "Data\\Day1.txt";

    bool Part1()
    {
        Stats timer(1, 1);

        std::ifstream file(data_file);
        if (!file.is_open())
            return false;

        int count = -1;
        int prev = -1;
        while (!file.eof())
        {
            int depth;
            file >> depth;
            if (depth > prev)
                count++;
            prev = depth;
        }

        std::cout << count << "\n";
        return true;
    }

    bool Part2()
    {
        Stats timer(1, 2);

        std::ifstream file(data_file);
        if (!file.is_open())
            return false;

        // Init first 3
        int depths[3];
        int window = 0;
        for (int i = 0; i < 3; i++)
        {
            file >> depths[i];
            window += depths[i];
        }

        int count = 0;
        int i = 0;
        while (!file.eof())
        {
            int prev = window;
            window -= depths[i];
            file >> depths[i];
            window += depths[i];
            i = (i + 1) % 3;
            if (window > prev)
                count++;
        }

        std::cout << count << "\n";
        return true;
    }

    extern void Both()
    {
        if (Part1())
            Part2();
    }
}