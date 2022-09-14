#include <iostream>
#include <fstream>
#include <string>

// https://adventofcode.com/2021/day/1
namespace Day1
{
    void Part1()
    {
        auto path = "Data\\Day1.txt";
        std::ifstream file(path);
        if (!file.is_open())
            return;

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

        std::cout << "Part 1\n";
        std::cout << count << "\n";
    }
}