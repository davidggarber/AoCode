#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

//using namespace std;

bool Part1()
{
    auto path = "Data\\Day1.txt";
    std::ifstream file(path);
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

    std::cout << "Part 1\n";
    std::cout << count << "\n";
    return true;
}

bool Part2()
{
    auto path = "Data\\Day1.txt";
    std::ifstream file(path);
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

    std::cout << "Part 2\n";
    std::cout << count << "\n";
    return true;
}

extern void Day1()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Day 1\n";
    if (Part1())
        Part2();


    auto finish = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / 1000000.0;
    std::cout << "Total time: " << ms << "ms\n";
}