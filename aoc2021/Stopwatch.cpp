#include "Stopwatch.h"

#include <iostream>
#include <windows.h>
#include <Psapi.h>


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

Stats::Stats(int day, int part)
{
    _day = day;
    _part = part;
}

Stats::~Stats()
{
    // At the end of each part, report the stats
    std::cout << "Day " << _day << "." << _part 
        << " : " << Mark() << "ms; " 
        << MeasureMemory() << "mb\n";
}

double Stats::MeasureMemory()
{
    PROCESS_MEMORY_COUNTERS memCounter;
    BOOL result = K32GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
    //std::cout << "WorkingSetSize " << memCounter.WorkingSetSize << std::endl;
    return memCounter.WorkingSetSize / (1024 * 1024.0);  // megabytes, as a decimal
}
