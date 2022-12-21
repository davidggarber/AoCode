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

// https://adventofcode.com/2022/day/14
namespace Day14
{
    class Data
    {
    public:
        vector<string> cave;

        void Set(int row, int col, char ch)
        {
            if (col < 0 || col >= 1000)
            {
                cout << "Unexpected coord " << row << ',' << col << "\n";
                return;
            }
            while (cave.size() <= row)
            {
                static string blank_row = string(1000, '.');
                cave.push_back(blank_row);
            }
            string s = cave[row];
            cave[row] = s.substr(0, col) + ch + s.substr(col + 1);
        }

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);
                    stringstream ss(line);
                    int r, c;
                    char comma;
                    ss >> c >> comma >> r;
                    Set(r, c, '#');
                    while (!ss.eof())
                    {
                        string arrow;
                        int row, col;
                        ss >> arrow >> col >> comma >> row;
                        int dc = (col > c) ? 1 : (col < c) ? -1 : 0;
                        int dr = (row > r) ? 1 : (row < r) ? -1 : 0;
                        while (r != row || c != col)
                        {
                            r += dr;
                            c += dc;
                            Set(r, c, '#');
                        }
                    }
                }
            }
        }

        void AddFloor()
        {
            string blank_row = string(1000, '.');
            cave.push_back(blank_row);
            string infinite_floor = string(1000, '#');
            cave.push_back(infinite_floor);
        }

        void Print(int first_col, int last_col)
        {
            for (int i = 0; i < cave.size(); i++)
            {
                string s = cave[i].substr(first_col, last_col - first_col);
                cout << s << "\n";
            }
        }

        bool Drop()
        {
            int c = 500;
            int r = 0;
            if (cave[r][c] != '.')
                return false;
            while (r + 1 < cave.size())
            {
                if (cave[r + 1][c] == '.')
                {
                    r++;
                }
                else if (cave[r + 1][c - 1] == '.')
                {
                    r++;
                    c--;
                }
                else if (cave[r + 1][c + 1] == '.')
                {
                    r++;
                    c++;
                }
                else
                {
                    Set(r, c, 'o');
                    return true;
                }
            }
            return false;  // Falls forever
        }
    };

    size_t Part1()
    {
        Data data;
        int count = 0;
        while (data.Drop())
            count++;
        data.Set(0, 500, '+');
        //data.Print(490, 510);
        return count;
    }

    size_t Part2()
    {
        Data data;
        data.AddFloor();
        int count = 0;
        while (data.Drop())
            count++;
        data.Set(0, 500, '+');
        //data.Print(470, 530);
        return count;
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