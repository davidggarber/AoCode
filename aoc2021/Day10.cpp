#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Stopwatch.h"
#include <algorithm>

using namespace std;

// https://adventofcode.com/2021/day/10
namespace Day10
{
    auto data_file = "Data\\Day10.txt";

    class Data
    {
    public:
        vector<string> doc;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    doc.push_back(line);
                }
            }
        }

    };

    int Corruption(string s)
    {
        string stack = "";
        for (auto sit = s.begin(); sit != s.end(); sit++)
        {
            if (*sit == '(' || *sit == '[' || *sit == '{' || *sit == '<')
                stack += *sit;
            else if (*sit == ')' && stack.back() != '(')
                return 3;
            else if (*sit == ']' && stack.back() != '[')
                return 57;
            else if (*sit == '}' && stack.back() != '{')
                return 1197;
            else if (*sit == '>' && stack.back() != '<')
                return 25137;
            else
                stack.pop_back();
        }
        return 0;
    }

    size_t Completion(string s)
    {
        string stack = "";
        for (auto sit = s.begin(); sit != s.end(); sit++)
        {
            if (*sit == '(' || *sit == '[' || *sit == '{' || *sit == '<')
                stack += *sit;
            else
                stack.pop_back();
        }
        size_t score = 0;
        while (stack.size() > 0)
        {
            score *= 5;
            switch (stack.back())
            {
            case '(':
                score += 1;
                break;
            case '[':
                score += 2;
                break;
            case '{':
                score += 3;
                break;
            case '<':
                score += 4;
                break;
            }
            stack.pop_back();
        }
        return score;
    }

    int Part1()
    {
        Data data;
        int score = 0;
        for (auto it = data.doc.begin(); it != data.doc.end(); it++)
        {
            score += Corruption(*it);
        }
        return score;
    }

    size_t Part2()
    {
        Data data;
        vector<size_t> scores;
        for (auto it = data.doc.begin(); it != data.doc.end(); it++)
        {
            if (Corruption(*it) == 0)
            {
                size_t score = Completion(*it);
                if (score > 0)
                    scores.push_back(score);
            }
        }
        sort(scores.begin(), scores.end());
        return scores[scores.size() / 2];
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