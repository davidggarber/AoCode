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

// https://adventofcode.com/2022/day/13
namespace Day13
{
    class Packet
    {
    public:
        string raw;

        Packet(string r)
        {
            raw = r;
        }

        Packet(const Packet& p) = default;

        const Packet& operator=(const Packet& p)
        {
            raw = p.raw;
            return *this;
        }

        bool operator==(const Packet& p) const
        {
            return raw == p.raw;
        }

        bool IsListContent() const
        {
            int depth = 0;
            for (auto it = raw.begin(); it != raw.end(); it++)
            {
                if (*it == '[')
                    depth++;
                else if (*it == ']')
                    depth--;
                else if (*it == ',' && depth == 0)
                    return true;
            }
            if (depth != 0)
                cout << "Unbalanced packet: " << raw << "\n";
            return false;
        }

        bool IsSimple() const
        {
            return raw.size() == 0
                || (!IsListContent() && raw[0] != '[');
        }

        bool IsListContainer() const
        {
            return !IsListContent() && raw[0] == '[';
        }

        vector<Packet> AsList() const
        {
            vector<Packet> list;
            if (raw.size() == 0)
                return list;
            if (IsListContent())
            {
                // Current level is contents of a list
                string s = "";
                int depth = 0;
                for (auto it = raw.begin(); it != raw.end(); it++)
                {
                    if (*it == ',' && depth == 0)
                    {
                        list.push_back(Packet(s));
                        s = "";
                        continue;
                    }
                    if (*it == '[')
                        depth++;
                    else if (*it == ']')
                        depth--;
                    s += *it;
                }
                list.push_back(s);
            }
            else if (IsListContainer())
            {
                // Current level is a list
                string contents = raw.substr(1, raw.size() - 2);
                if (contents.size() > 0)
                    list.push_back(Packet(contents));
            }
            else
            {
                list.push_back(Packet(raw));
            }
            return list;
        }

        int AsValue() const
        {
            if (raw.size() == 0)
                return -1;
            return atoi(raw.c_str());
        }
    };

    class Data
    {
    public:
        vector<Packet> left, right;
        vector<Packet> sorted;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string first, second, blank;
                    getline(file, first);
                    getline(file, second);
                    if (!file.eof())
                        getline(file, blank);
                    left.push_back(Packet(first));
                    right.push_back(Packet(second));
                }
            }
        }

        int Insert(const Packet& packet)
        {
            int index = 0;
            for (auto it = sorted.begin(); it != sorted.end(); it++)
            {
                bool correct = false;
                index++;
                if (TestCorrect(packet, *it, correct) && correct)
                {
                    sorted.insert(it, packet);
                    return index;
                }
            }
            sorted.insert(sorted.end(), packet);
            return sorted.size();
        }

        bool TestCorrect(const Packet& f, const Packet& s, bool& correct)
        {
            if (f.IsSimple() && s.IsSimple())
            {
                auto fv = f.AsValue();
                auto sv = s.AsValue();
                correct = fv < sv;
                return fv != sv;
            }
            else //if (f.IsListContainer() || s.IsListContainer())
            {
                auto ff = f.AsList();
                auto ss = s.AsList();
                for (int i = 0; i < ff.size(); i++)
                {
                    if (i >= ss.size())
                    {
                        correct = false;
                        return true;
                    }
                    if (TestCorrect(ff[i], ss[i], correct))
                        return true;
                }
                if (ff.size() < ss.size())
                {
                    correct = true;
                    return true;
                }
                return false;  // identical lists
            }
        }

        bool IsCorrect(int index)
        {
            Packet f = left[index];
            Packet s = right[index];
            bool correct = false;
            return TestCorrect(f, s, correct) && correct;
        }
    };

    size_t Part1()
    {
        Data data;
        int sum = 0;
        for (int i = 0; i < data.left.size(); i++)
        {
            if (data.IsCorrect(i))
                sum += i + 1;
        }
        return sum;
    }

    size_t Part2()
    {
        Data data;
        for (int i = 0; i < data.left.size(); i++)
        {
            data.Insert(data.left[i]);
            data.Insert(data.right[i]);
        }
        int a = data.Insert(Packet("[[2]]"));
        int b = data.Insert(Packet("[[6]]"));
        return a * b;
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