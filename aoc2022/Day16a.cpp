#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_STEPS 30
#define TRACE_FREQ 1

// https://adventofcode.com/2022/day/16
namespace Day16a
{
    class Valve
    {
    public:
        long rate;
        vector<string> tunnels;

        Valve() = default;
        Valve(const Valve& v) = default;
        const Valve& operator=(const Valve& v)
        {
            rate = v.rate;
            tunnels = v.tunnels;
            return *this;
        }

        bool IsDeadEnd()
        {
            return tunnels.size() == 1;
        }
    };

    class Data
    {
    public:
        unordered_map<string, Valve*> valves;

        Data()
        {
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);

                    Valve* v = new Valve;
                    string valve, name, has, flow, rate, tunnel, leads, to, other;
                    char comma;
                    stringstream ss(line);
                    ss >> valve >> name >> has >> flow >> rate >> tunnel >> leads >> to >> valve;
                    v->rate = atoi(rate.substr(5, rate.size() - 6).c_str());
                    while (!ss.eof())
                    {
                        ss >> other;
                        v->tunnels.push_back(other.substr(0, 2));
                    }
                    valves[name] = v;
                }
            }
        }
    };

    class Branch
    {
    public:
        vector<string> options;
        int cur;

        Branch(vector<string> opts)
        {
            options = opts;
            cur = 0;
        }

        Branch(const Branch& b) = default;
        const Branch& operator=(const Branch& b)
        {
            options = b.options;
            cur = b.cur;
        }

        string Current()
        {
            return options[cur];
        }

        bool Next()
        {
            return ++cur < options.size();
        }

        int Remaining()
        {
            return options.size() - cur;
        }
    };

    class Route
    {
    public:
        vector<string> path;
        unordered_set<string> open;
        unordered_set<string> finished;
        unordered_set<string> visited;
        long flow_per_minute;
        long total_flow;

        Route(Data& data, vector<Branch>& branches, bool trace)
        {
            auto it = branches.begin();
            path.push_back(it->Current());

            if (trace)
                Trace(data, *it);

            for (it++; it != branches.end(); it++)
            {
                if (trace)
                    Trace(data, *it);
                Continue(data, it->Current());
            }
            if (trace)
                cout << " ==> " << total_flow << " @ " << flow_per_minute << "\n";
        }

        Route(string start)
        {
            path.push_back(start);
            flow_per_minute = total_flow = 0;
            // Any flow=zeros are effectively already open
            //for (auto it = zeros.begin(); it != zeros.end(); it++)
            //    open.insert(*it);
        }

        Route(const Route& r)
        {
            path = r.path;
            open = r.open;
            finished = r.finished;
            flow_per_minute = r.flow_per_minute;
            total_flow = r.total_flow;
        }

        const Route& operator=(const Route& r)
        {
            path = r.path;
            open = r.open;
            finished = r.finished;
            flow_per_minute = r.flow_per_minute;
            total_flow = r.total_flow;
            return *this;
        }

        string Current()
        {
            return path.back();
        }

        void Trace(Data& data, Branch& br)
        {
            string f = br.Current();
            if (f == "~")
                cout << "~" << Current()[0];
            else if (f == "." || !IsOpen(data, f))
                cout << f;
            else
                cout << (char)tolower(f[0]);
            cout << "(" << br.Remaining() << ") ";
        }

        bool IsOpen(Data& data, string name)
        {
            return data.valves[name]->rate == 0 || open.find(name) != open.end();
        }

        bool IsOpen(Data& data)
        {
            return IsOpen(data, Current());
        }

        bool IsFinished(string tunnel)
        {
            return finished.find(tunnel) != finished.end();
        }

        vector<string> Options(Data& data, string prev)
        {
            vector<string> options;
            if (!IsOpen(data))
                options.push_back("~");  // Special code means open
            for (auto it = data.valves[Current()]->tunnels.begin(); it != data.valves[Current()]->tunnels.end(); it++)
            {
                // Don't return to a tunnel having changed nothing
                string status = *it + to_string(flow_per_minute);
                if (visited.find(status) != visited.end())
                    continue;

                // Don't go to tunnels we can't improve
                // Also don't turn around and go back where we just came from if we didn't do anything interesting
                if (!IsFinished(*it) && *it != prev)
                    options.push_back(*it);
            }
            if (options.size() == 0)
                options.push_back(".");  // Sit around
            return options;
        }

        void Continue(Data& data, string dest)
        {
            total_flow += flow_per_minute;
            if (dest == "~")
            {
                open.insert(Current());
                flow_per_minute += data.valves[Current()]->rate;
            }
            else if (dest != ".")
            {
                bool done = IsOpen(data);
                for (auto it = data.valves[Current()]->tunnels.begin(); done && it != data.valves[Current()]->tunnels.end(); it++)
                {
                    if (*it != dest && !IsFinished(*it))
                        done = false;
                }
                if (done)
                {
                    finished.insert(Current());
                }
                path.push_back(dest);
            }
            string status = Current() + to_string(flow_per_minute);
            visited.insert(status);
        }

        Route Continue(Data& data, vector<string>& options, int index)
        {
            Route next(*this);
            next.total_flow += next.flow_per_minute;
            if (options[index] == "~")
            {
                next.open.insert(Current());
                next.flow_per_minute += data.valves[Current()]->rate;
            }
            else if (options[index] != ".")
            {
                string dest = options[index];
                bool done = IsOpen(data);
                for (auto it = data.valves[Current()]->tunnels.begin(); done && it != data.valves[Current()]->tunnels.end(); it++)
                {
                    if (*it != dest && !IsFinished(*it))
                        done = false;
                }
                if (done)
                {
                    next.finished.insert(Current());
                }
                next.path.push_back(dest);
            }
            return next;
        }
    };

    long Part1()
    {
        Data data;
        long best = 0;

        vector<string> start;
        start.push_back("AA");

        vector<Branch> stack;
        stack.push_back(Branch(start));

        int tracer = 0;

        while (stack.size() > 0)
        {
            bool trace = stack.size() == MAX_STEPS + 1;
            if (trace)
            {
                trace &= (++tracer % TRACE_FREQ) == 0;
            }

            Route r(data, stack, false);
            if (stack.size() < MAX_STEPS + 1) // Initial AA doesn't count
            {
                string prev = stack.size() <= 1 ? "" : stack[stack.size() - 2].Current();
                vector<string> options = r.Options(data, prev);  // Always at least 1 (sit)
                stack.push_back(Branch(options));
            }
            else
            {
                if (r.total_flow > best)
                {
                    Route rr(data, stack, true);
                    best = r.total_flow;
                }
                stack.pop_back();
                while (stack.size() > 0 && !stack.back().Next())
                {
                    stack.pop_back();
                }
            }
        }

        return best;
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