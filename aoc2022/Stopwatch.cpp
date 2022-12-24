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
    long ms = std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000;
    return ms / 1000.0;  // seconds, with 3 decimals
    //return std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000.0;
}

Stats::Stats(std::string nmsp, int part)
{
    _nmsp = nmsp;
    _part = part;
}

Stats::~Stats()
{
    // At the end of each part, report the stats
    std::cout << _nmsp << ", part " << _part
        << " : " << Mark() << "s; " 
        << MeasureMemory() << "mb\n";
}

double Stats::MeasureMemory()
{
    PROCESS_MEMORY_COUNTERS memCounter;
    BOOL result = K32GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
    //std::cout << "WorkingSetSize " << memCounter.WorkingSetSize << std::endl;
    return memCounter.WorkingSetSize / (1024 * 1024.0);  // megabytes, as a decimal
}
