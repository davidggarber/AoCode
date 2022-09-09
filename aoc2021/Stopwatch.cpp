#include "Stopwatch.h"

#include <iostream>

Stopwatch::Stopwatch()
{
    // Start a stopwatch
    _start = std::chrono::high_resolution_clock::now();
}

double Stopwatch::Mark(bool restart)
{
    // Mark the time on a stopwatch
    auto finish = std::chrono::high_resolution_clock::now();
    // Measure the time passed
    auto delta = finish - _start;
    if (restart)
        _start = finish;  // Move start time to now, if asked
    // Return the ellapsed time in milliseconds
    return std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000.0;
}

StopwatchConsole::StopwatchConsole(int day, int part)
{
    _day = day;
    _part = part;
}

StopwatchConsole::~StopwatchConsole()
{
    // At the end of each part, report the stats
    std::cout << "Day " << _day << "." << _part 
        << " : " << Mark() << "ms\n";
}