#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Stopwatch.h"

using namespace std;

// https://adventofcode.com/2021/day/4
namespace Day4
{
    auto data_file = "Data\\Day4.txt";

    class Board
    {
    public:
        int numbers[25];
        bool Read(ifstream& file)
        {
            for (int i = 0; i < 25; i++)
            {
                file >> numbers[i];
            }
            return true;
        }
        bool check(int pull)
        {
            for (int i = 0; i < 25; i++)
            {
                if (numbers[i] == pull)
                {
                    numbers[i] = -1;
                    return bingo();
                }
            }
            return false;
        }
        bool bingo()
        {
            for (int r = 0; r < 5; r++)
            {
                if (bingoRow(r, 5) || bingoRow(r * 5, 1))
                    return true;
            }
            return bingoRow(0, 6) || bingoRow(4, 4);
        }
        bool bingoRow(int root, int step)
        {
            int product = numbers[root];
            for (int i = 1; i < 5; i++)
            {
                root += step;
                product *= numbers[root];
            }
            return product == -1;
        }
        int sumUndrawn()
        {
            int sum = 0;
            for (int i = 0; i < 25; i++)
            {
                if (numbers[i] >= 0)
                    sum += numbers[i];
            }
            return sum;
        }
    };

    class Data
    {
    public:
        vector<int> pulls;
        vector<Board> boards;

        Data()
        {
            ifstream file(data_file);
            if (file.is_open())
            {
                string line;
                file >> line;
                stringstream ss(line);
                int pull;
                while (ss >> pull)
                {
                    pulls.push_back(pull);
                    if (ss.peek() == ',') {
                        ss.ignore();
                    }
                }

                while (!file.eof())
                {
                    Board b;
                    if (b.Read(file))
                        boards.push_back(std::move(b));
                }
            }
        }
    };

    int Part1()
    {
        Data data;

        for (auto pit = data.pulls.begin(); pit != data.pulls.end(); pit++)
        {
            int pull = *pit;
            for (auto b = data.boards.begin(); b != data.boards.end(); b++)
            {
                if (b->check(pull))
                    return pull * b->sumUndrawn();
            }
        }

        return 0;
    }

    int Part2()
    {
        Data data;

        for (auto pit = data.pulls.begin(); pit != data.pulls.end(); pit++)
        {
            int pull = *pit;
            for (int b = data.boards.size() - 1; b >= 0; b--)
            {
                if (data.boards[b].check(pull))
                {
                    if (data.boards.size() == 1)
                        return pull * data.boards[b].sumUndrawn();
                    data.boards.erase(data.boards.begin() + b);
                }
            }
        }

        return 0;
    }

    /// <summary>
    /// Run parts 1 and 2.
    /// Time each of them.
    /// </summary>
    extern void Both()
    {
        {
            Stats timer(1, 1);
            cout << Part1() << "\n";
        }
        {
            Stats timer(1, 2);
            cout << Part2() << "\n";
        }
    }
}