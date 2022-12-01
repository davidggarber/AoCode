#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/8
namespace Day8
{
    auto data_file = "Data\\Day8.txt";

    class Config
    {
    public:
        vector<string> digits;
        vector<string> output;

        int KnownValue(string& s)
        {
            if (s.size() == 2)
                return 1;
            if (s.size() == 3)
                return 7;
            if (s.size() == 4)
                return 4;
            if (s.size() == 7)
                return 8;
            return 0;
        }

        string FindDigit(int val)
        {
            for (int i = 0; i < 10; i++)
            {
                if (KnownValue(digits[i]) == val)
                    return digits[i];
            }
            return "";
        }

        int CountCommon(string set, string subset)
        {
            int count = 0;
            for (int i = 0; i < subset.size(); i++)
            {
                size_t f = set.find(subset[i]);
                if (f >= 0 && f < set.size())
                    count++;
            }
            return count;
        }

        bool HasAll(string set, string subset)
        {
            return CountCommon(set, subset) == subset.size();
        }

        int DigitValue(int index)
        {
            string o = output[index];
            int val = KnownValue(o);
            if (val > 0)
                return val;
            if (output[index].size() == 5)
            {
                if (HasAll(o, FindDigit(1)))
                    return 3;  // 3 is a superset of 1
                if (CountCommon(o, FindDigit(4)) == 3)
                    return 5;  // 5 uses most of a 4
                return 2;  // uses less of a 4
            }
            else //if (output[index].size() == 6)
            {
                if (HasAll(o, FindDigit(4)))
                    return 9;  // 9 is a superset of 4
                if (HasAll(o, FindDigit(7)))
                    return 0;  // 0 and 9 are supersets of 7
                return 6;
            }
        }

        int Output()
        {
            return DigitValue(0) * 1000
                + DigitValue(1) * 100
                + DigitValue(2) * 10
                + DigitValue(3);
        }
    };

    class Data
    {
    public:
        vector<Config> configs;

        void Normalize(string& s)
        {
            sort(s.begin(), s.end());
        }

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                while (!file.eof())
                {
                    Config config;
                    string s;
                    for (int i = 0; i < 10; i++)
                    {
                        file >> s;
                        Normalize(s);
                        config.digits.push_back(s);
                    }

                    file >> s;  // |

                    for (int i = 0; i < 4; i++)
                    {
                        file >> s;
                        Normalize(s);
                        config.output.push_back(s);
                    }

                    configs.push_back(std::move(config));
                }
            }
        }
    };

    size_t Part1()
    {
        Data data;
        int count = 0;
        for (auto it = data.configs.begin(); it != data.configs.end(); it++)
        {
            for (int i = 0; i < 4; i++)
            {
                int val = it->DigitValue(i);
                if (val > 0)
                    count++;
            }
        }

        return count;
    }

    size_t Part2()
    {
        Data data;
        int sum = 0;
        for (auto it = data.configs.begin(); it != data.configs.end(); it++)
        {
            int out = it->Output();
            sum += out;
        }
        return sum;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(8, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(8, 2);
            cout << Part2() << "\n";
        }
    }
}