#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

// https://adventofcode.com/2022/day/25
namespace Day25
{
    int64_t Snafu2Dec(char c)
    {
        switch (c)
        {
        case '=':
            return -2;
        case '-':
            return -1;
        default:
            return c - '0';
        }
    }

    int64_t Snafu2Dec(string snafu)
    {
        int64_t place = 1;
        int64_t dec = 0;
        auto it = snafu.end();
        while (it != snafu.begin())
        {
            it--;
            int64_t s = Snafu2Dec(*it);
            dec += place * s;
            place *= 5;
        }
        return dec;
    }

    string Dec2Snafu(const vector<int64_t>& vec)
    {
        string snafu = "";
        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            switch (*it)
            {
            case -2:
                snafu = "=" + snafu;
                break;
            case -1:
                snafu = "-" + snafu;
                break;
            default:
                snafu = to_string(*it) + snafu;
                break;
            }
        }
        return snafu;
    }

    string Dec2Snafu(int64_t dec)
    {
        vector<int64_t> snafu;
        int64_t mod = 5;
        do
        {
            int64_t d = dec % mod;
            if (d >= 0 && d <= 2)
            {
                snafu.push_back(d);
                dec -= d;
            }
            else
            {
                snafu.push_back(d - 5);
                dec -= d - 5;
            }
            dec /= mod;
        } while (dec != 0);
        return Dec2Snafu(snafu);
    }


    class Data
    {
    public:
        vector<int64_t> fuel;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);
                    int64_t dec = Snafu2Dec(line);
                    fuel.push_back(dec);
                }
            }
        }
    };

    string Part1()
    {
        Data data;
        int64_t total = 0;
        for (auto it = data.fuel.begin(); it != data.fuel.end(); it++)
            total += *it;
        return Dec2Snafu(total);
    }

    size_t Part2()
    {
        Data data;
        return 0;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(DayFromNamespace(__FUNCSIG__), 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(DayFromNamespace(__FUNCSIG__), 2);
            cout << Part2() << "\n";
        }
    }
}