#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Stopwatch.h"

// https://adventofcode.com/2021/day/3
namespace Day3
{
    auto data_file = "Data\\Day3.txt";
    const int bit_count = 12;

    struct Diagnostic
    {
        std::string bits;
        int bit(int place)
        {
            return bits[place] == '1' ? 1 : 0;
        }
    };

    bool lessThan(const Diagnostic& a, const Diagnostic& b) 
    {
        return a.bits.compare(b.bits) < 0;
    }

    class Data
    {
    public:
        std::vector<Diagnostic> lines;

        Data()
        {
            std::ifstream file(data_file);
            if (file.is_open())
            {
                while (!file.eof())
                {
                    Diagnostic row;
                    file >> row.bits;
                    lines.push_back(row);
                }
            }
        }
    };

    int Part1()
    {
        Data data;
        int frequencies[bit_count] = { 0 };
        for (int i = 0; i < data.lines.size(); i++)
        {
            for (int p = 0; p < bit_count; p++)
            {
                frequencies[p] += data.lines[i].bit(p);
            }
        }

        int gamma = 0, epsilon = 0;
        for (int p = 0; p < bit_count; p++)
        {
            gamma <<= 1;
            gamma |= (frequencies[p] * 2 >= data.lines.size()) ? 1 : 0;
        }
        epsilon = ~gamma & ((1 << bit_count) - 1);
        return epsilon * gamma;
    }

    // Find the first index in the vector that prefix-matches
    int findFirst(const std::vector<Diagnostic>& lines, int begin, int end, const std::string& prefix)
    {
        while (end > begin)
        {
            int middle = (begin + end) / 2;
            int comp = lines[middle].bits.find(prefix) == 0
                ? 0 : lines[middle].bits.compare(prefix);
            if (comp < 0)
                begin = middle + 1;
            else
                end = middle;
        }
        return end;
    }

    // Find the first index in the vector past any prefix-matches
    int findLast(const std::vector<Diagnostic>& lines, int begin, int end, const std::string& prefix)
    {
        while (end > begin)
        {
            int middle = (begin + end) / 2;
            int comp = lines[middle].bits.find(prefix) == 0
                ? 0 : lines[middle].bits.compare(prefix);
            if (comp <= 0)
                begin = middle + 1;
            else
                end = middle;
        }
        return end;
    }

    int Part2()
    {
        Data data;

        // Calculate oxygen
        int ox = 0;
        {
            std::string mostCommon = "";
            std::vector<Diagnostic> lines(data.lines);
            std::sort(lines.begin(), lines.end(), lessThan);
            int begin = 0;
            int end = (int)lines.size();
            for (int p = 0; p < bit_count && begin + 1 < end; p++)
            {
                int middle = (begin + end) / 2;
                // The most common bit is the middle. In a tie, take the 1 (sorted later)
                mostCommon += lines[middle].bits[p];
                // Binary search from the middle, since we know middle is included
                begin = findFirst(lines, begin, middle + 1, mostCommon);
                end = findLast(lines, middle, end, mostCommon);
            }
            for (int p = 0; p < bit_count; p++)
                ox = (ox << 1) | lines[begin].bit(p);
        }

        // Calculate CO2
        int co2 = 0;
        {
            std::string leastCommon = "";
            std::vector<Diagnostic> lines(data.lines);
            std::sort(lines.begin(), lines.end(), lessThan);
            int begin = 0;
            int end = (int)lines.size();
            for (int p = 0; p < bit_count && begin + 1 < end; p++)
            {
                int middle = (begin + end) / 2;
                // The most common bit is the middle. In a tie, take the 1 (sorted later)
                int bit = lines[middle].bit(p);
                leastCommon += bit == 1 ? '0' : '1';
                // Binary search from the ends, since we know middle is not included
                begin = findFirst(lines, begin, end, leastCommon);
                end = findLast(lines, begin, end, leastCommon);
            }
            for (int p = 0; p < bit_count; p++)
                co2 = (co2 << 1) | lines[begin].bit(p);
        }

        return ox * co2;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    void Both()
    {
        {
            Stats timer(3, 1);
            std::cout << Part1() << "\n";
        }
        {
            Stats timer(3, 2);
            std::cout << Part2() << "\n";
        }
    }
}