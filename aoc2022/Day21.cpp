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

// https://adventofcode.com/2022/day/21
namespace Day21
{
    class Monkey
    {
    public:
        int64_t value;
        bool human_branch;
        string first, second;
        string parent;
        char oper;
    };

    class Data
    {
    public:
        unordered_map<string, Monkey*> doc;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                string line;
                while (!file.eof())
                {
                    getline(file, line);
                    string name = line.substr(0, 4);
                    Monkey* m = new Monkey();
                    m->parent = "";
                    m->human_branch = name == "humn";
                    if (line.size() > 10)
                    {
                        m->value = 0;
                        m->first = line.substr(6, 4);
                        m->oper = line[11];
                        m->second = line.substr(13, 4);
                    }
                    else
                    {
                        m->value = atoi(line.substr(6).c_str());
                        m->oper = '\0';
                    }
                    doc[name] = m;
                }
            }
        }

        bool HumanFirstChild(string name)
        {
            if (name == "humn")
                return true;
            Monkey* m = doc[name];
            if (m->human_branch)
                return doc[m->first]->human_branch;
            return false;
        }

        int64_t Eval(string name)
        {
            Monkey* m = doc[name];
            if (m->oper == '\0' || m->value != 0)
                return m->value;
            if (doc[m->first]->parent == "")
                doc[m->first]->parent = name;
            else
                cout << m->first << " already has parent " << doc[m->first]->parent << " not " << name;
            if (doc[m->second]->parent == "")
                doc[m->second]->parent = name;
            else
                cout << m->second << " already has parent " << doc[m->second]->parent << " not " << name;

            int64_t f = Eval(m->first);
            int64_t s = Eval(m->second);
            m->human_branch = doc[m->first]->human_branch || doc[m->second]->human_branch;

            switch (m->oper)
            {
            case '+':
                return m->value = f + s;
            case '-':
                return m->value = f - s;
            case '/':
                return m->value = f / s;
            case '*':
                return m->value = f * s;
            case '=':
                return m->value = f == s;
            default:
                cout << "unknown operator: " << m->oper;
                return 0;
            }
        }

        void Fix(string name, int64_t new_value)
        {
            Monkey* m = doc[name];

            switch (m->oper)
            {
            case '\0':
                m->value = new_value;
                return;
            case '+':
                if (HumanFirstChild(name))
                    Fix(m->first, new_value - Eval(m->second));
                else
                    Fix(m->second, new_value - Eval(m->first));
                return;
            case '-':
                if (HumanFirstChild(name))
                    Fix(m->first, new_value + Eval(m->second));
                else
                    Fix(m->second, Eval(m->first) - new_value);
                return;
            case '/':
                if (HumanFirstChild(name))
                    Fix(m->first, new_value * Eval(m->second));
                else
                    Fix(m->second, Eval(m->first) / new_value);
                return;
            case '*':
                if (HumanFirstChild(name))
                    Fix(m->first, new_value / Eval(m->second));
                else
                    Fix(m->second, new_value / Eval(m->first));
                return;
            case '=':
                if (HumanFirstChild(name))
                    Fix(m->first, Eval(m->second));
                else
                    Fix(m->second, Eval(m->first));
                return;
            default:
                cout << "unknown operator: " << m->oper;
                return;
            }
        }
    };

    int64_t Part1()
    {
        Data data;
        int64_t root = data.Eval("root");
        return root;
    }

    size_t Part2()
    {
        Data data;
        data.doc["root"]->oper = '=';
        data.Eval("root");
        data.Fix("root", 0);
        return data.Eval("humn");
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