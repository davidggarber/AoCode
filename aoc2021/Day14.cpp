#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/14
namespace Day14
{
    auto data_file = "Data\\Day14.txt";

    class Data
    {
    public:
        unordered_map<string, string> rules;
        string start;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                getline(file, start);

                string line;
                getline(file, line);  // blank
                while (!file.eof())
                {
                    getline(file, line);
                    string before = line.substr(0, 2);
                    string after = line.substr(0, 1) + line.substr(6, 1) + line.substr(1, 1);
                    rules[before] = after;
                }
            }
        }

    };

    size_t Part1()
    {
        Data data;
        unordered_map<string, size_t> state;
        for (int i = 1; i < data.start.size(); i++)
        {
            string sub = data.start.substr(i - 1, 2);
            state[sub] += 1;
        }

        for (int i = 0; i < 10; i++)
        {
            unordered_map<string, size_t> next;
            for (auto it = state.begin(); it != state.end(); it++)
            {
                string a = data.rules[it->first].substr(0, 2);
                string b = data.rules[it->first].substr(1, 2);
                next[a] += it->second;
                next[b] += it->second;
            }
            state = next;
        }

        unordered_map<char, size_t> freq;
        for (auto it = state.begin(); it != state.end(); it++)
        {
            //char a = it->first[0];
            char b = it->first[1];
            //freq[a] += it->second;
            freq[b] += it->second;
        }
        freq[data.start[0]]++;
        size_t most = 0;
        size_t least = data.start.size() << 10;
        for (auto it = freq.begin(); it != freq.end(); it++)
        {
            if (it->second > most)
                most = it->second;
            if (it->second < least)
                least = it->second;
        }

        return most - least;
    }

    size_t Part2()
    {
        Data data;
        unordered_map<string, size_t> state;
        for (int i = 1; i < data.start.size(); i++)
        {
            string sub = data.start.substr(i - 1, 2);
            state[sub] += 1;
        }

        for (int i = 0; i < 40; i++)
        {
            unordered_map<string, size_t> next;
            for (auto it = state.begin(); it != state.end(); it++)
            {
                string a = data.rules[it->first].substr(0, 2);
                string b = data.rules[it->first].substr(1, 2);
                next[a] += it->second;
                next[b] += it->second;
            }
            state = next;
        }

        unordered_map<char, size_t> freq;
        for (auto it = state.begin(); it != state.end(); it++)
        {
            //char a = it->first[0];
            char b = it->first[1];
            //freq[a] += it->second;
            freq[b] += it->second;
        }
        freq[data.start[0]]++;
        size_t most = 0;
        size_t least = data.start.size() << 40;
        for (auto it = freq.begin(); it != freq.end(); it++)
        {
            if (it->second > most)
                most = it->second;
            if (it->second < least)
                least = it->second;
        }

        return most - least;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(9, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(9, 2);
            cout << Part2() << "\n";
        }
    }
}