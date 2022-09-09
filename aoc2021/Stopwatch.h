#pragma once

#include <chrono>

/// <summary>
/// Wrapper for chrono::high_resolution_clock.
/// Used for timing durations for performance comparisons.
/// </summary>
class Stopwatch
{
public:
	/// <summary>
	/// Construct and also start the timer
	/// </summary>
	Stopwatch();

	/// <summary>
	/// Measure ellapsed time in milliseconds since
	/// </summary>
	/// <param name="restart">If true, also sets new start time to now</param>
	/// <returns>Ellapsed time in milliseconds</returns>
	double Mark(bool restart = false);

private:
	std::chrono::steady_clock::time_point _start;
};

/// <summary>
/// Wrapper for chrono::high_resolution_clock.
/// Used for timing durations for performance comparisons.
/// Will automatically output statistics to cout in destructor.
/// </summary>
class Stats : public Stopwatch
{
public:
	/// <summary>
	/// Construct and start a stopwatch.
	/// </summary>
	/// <param name="day">The challenge's day - 1 to 25</param>
	/// <param name="part">The challenge part within the day - 1 or 2</param>
	Stats(int day, int part);
	
	/// <summary>
	/// Measures ellapsed time.
	/// Writes stats to cout, labelling with {day}.{part}
	/// </summary>
	virtual ~Stats();

	/// <summary>
	/// Calculate how much memory the program is using at this moment
	/// </summary>
	/// <returns>How many megabytes of working set</returns>
	double MeasureMemory();

protected:
	int _day;
	int _part;
};