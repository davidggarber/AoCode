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

// https://adventofcode.com/2022/day/8
namespace Day8
{
    struct Visible
    {
        bool l, t, r, b;

        bool IsVisible()
        {
            return l || r || t || b;
        }
    };

    class Data
    {
    public:
        vector<string> data;
        int size;
        Visible* vis;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    file >> line;
                    data.push_back(line);
                }
            }

            size = data.size() * data[0].size();
        }

        int HeightAt(int r, int c)
        {
            return data[r][c];
        }

        Visible* VisAt(int r, int c)
        {
            return &vis[r * data[0].size() + c];
        }

        void ScanEdges()
        {
            vis = new Visible[size];
            memset(vis, 0, size * sizeof(Visible));
            for (int r = 0; r < data.size(); r++)
            {
                string& s = data[r];
                int max = -1;
                for (int i = 0; i < s.size(); i++)
                {
                    auto v = VisAt(r, i);
                    v->l = s[i] > max;
                    max = (max < s[i]) ? s[i] : max;
                }

                max = -1;
                for (int i = s.size() - 1; i >= 0; i--)
                {
                    auto v = VisAt(r, i);
                    v->r = s[i] > max;
                    max = (max < s[i]) ? s[i] : max;
                }
            }

            for (int c = 0; c < data[0].size(); c++)
            {
                int max = -1;
                for (int i = 0; i < data.size(); i++)
                {
                    auto v = VisAt(i, c);
                    v->t = data[i][c] > max;
                    max = (max < data[i][c]) ? data[i][c] : max;
                }

                max = -1;
                for (int i = data.size() - 1; i >= 0; i--)
                {
                    auto v = VisAt(i, c);
                    v->b = data[i][c] > max;
                    max = (max < data[i][c]) ? data[i][c] : max;
                }
            }
        }

        int CountVisible()
        {
            ScanEdges();
            int count = 0;
            for (int i = 0; i < size; i++)
            {
                if (vis[i].IsVisible())
                    count++;
            }
            return count;
        }

        int MostScenic()
        {
            int maxCount = 0;
            int maxScore = 0;
            for (int r = 0; r < data.size(); r++)
            {
                for (int c = 0; c < data[0].size(); c++)
                {
                    int h = HeightAt(r, c);
                    int total = 0;
                    int score = 1;
                    int count = 0;
                    for (int cc = c - 1; cc >= 0; cc--)
                    {
                        count++;
                        if (HeightAt(r, cc) >= h)
                            break;
                    }
                    score *= count;
                    total += count;
                    count = 0;
                    for (int cc = c + 1; cc < data[r].size(); cc++)
                    {
                        count++;
                        if (HeightAt(r, cc) >= h)
                            break;
                    }
                    score *= count;
                    total += count;
                    count = 0;
                    for (int rr = r - 1; rr >= 0; rr--)
                    {
                        count++;
                        if (HeightAt(rr, c) >= h)
                            break;
                    }
                    score *= count;
                    total += count;
                    count = 0;
                    for (int rr = r + 1; rr < data.size(); rr++)
                    {
                        count++;
                        if (HeightAt(rr, c) >= h)
                            break;
                    }
                    score *= count;
                    total += count;
                    if (score > maxScore)
                    {
                        maxCount = count;
                        maxScore = score;
                    }
                }
            }
            return maxScore;
        }
    };

    size_t Part1()
    {
        Data data;
        return data.CountVisible();
    }

    size_t Part2()
    {
        Data data;
        return data.MostScenic();
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