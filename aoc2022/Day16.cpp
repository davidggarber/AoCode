#include "Utils.h"
#include "Stopwatch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define MAX_STEPS 30
#define TRACE_FREQ 1

// https://adventofcode.com/2022/day/16
namespace Day16
{
    class Tunnel
    {
    public:
        string dest;
        long distance;

        Tunnel() = default;
        Tunnel(const Tunnel& t) = default;
        Tunnel(string de, long di)
            : dest(de)
            , distance(di)
        {}

        const Tunnel& operator=(const Tunnel& t)
        {
            dest = t.dest;
            distance = t.distance;
            return *this;
        }

        bool operator==(const Tunnel& t) const
        {
            return dest == t.dest && distance == t.distance;
        }

        // Prioritize shorter distances and earlier names
        bool operator<(const Tunnel& t) const
        {
            return distance > t.distance
                || (distance == t.distance && dest > t.dest);
        }
    };

    class Valve
    {
    public:
        string name;
        long rate;
        unordered_map<string, long> tunnels;
        unordered_map<string, string> intermediates;

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

        /*
        * IDEA
        * While reducing the map, document the non-zero tunnels that are crossed along the way
        * (there may be multiple paths - document all)
        * Then when choosing to go from AA->HH without turning on EE, we know we're making that choice.
        * If HH is 5 steps away vs. EE is 2, the value of HH needs to be 3x more, if we know we'll double back.
        * Or 6x more if we wouldn't otherwise double back.
        * Thus, starting at AA, it's never worth skipping over DD, but it is worth skipping over EE to get to HH.
        */

        void RemoveZeros(const unordered_map<string, Valve*>& valves)
        {
            unordered_map<string, long> best;
            best[name] = 0;
            priority_queue<Tunnel> zeros;
            for (auto it = tunnels.begin(); it != tunnels.end(); it++)
            {
                //best[it->first] = it->second;
                //if (valves.at(it->first)->rate == 0)
                zeros.push(Tunnel(it->first, it->second));
            }
            tunnels.clear();
            while (zeros.size() > 0)
            {
                Tunnel t = zeros.top();
                zeros.pop();
                Valve* v = valves.at(t.dest);
                auto bit = best.find(v->name);
                if (bit != best.end() && t.distance >= bit->second)
                    continue;
                best[v->name] = t.distance;
                if (v->rate > 0)
                    tunnels[v->name] = t.distance;
                for (auto it = v->tunnels.begin(); it != v->tunnels.end(); it++)
                {
                    zeros.push(Tunnel(it->first, t.distance + it->second));
                }
            }
        }
    };

    class Data
    {
    public:
        unordered_map<string, Valve*> valves;
        int working_valves = 0;
        int max_flow = 0;

        Data()
        {

        }

        void Init()
        {
            valves.clear();
            ifstream file(FileFromNamespace(__FUNCSIG__));
            if (file.is_open())
            {
                while (!file.eof())
                {
                    string line;
                    getline(file, line);

                    Valve* v = new Valve;
                    string valve, name, has, flow, rate, tunnel, leads, to, other;
                    stringstream ss(line);
                    ss >> valve >> v->name >> has >> flow >> rate >> tunnel >> leads >> to >> valve;
                    v->rate = atoi(rate.substr(5, rate.size() - 6).c_str());
                    while (!ss.eof())
                    {
                        ss >> other;
                        string dest = other.substr(0, 2);
                        v->tunnels[dest] = 1;
                    }
                    valves[v->name] = v;
                }

                max_flow = 0;
                for (auto it = valves.begin(); it != valves.end(); it++)
                {
                    max_flow += it->second->rate;
                    it->second->RemoveZeros(valves);
                }
                working_valves = valves["AA"]->tunnels.size();
                if (valves["AA"]->rate > 0)
                    working_valves++;
            }
        }

        unordered_set<string> InterestingValves(const unordered_set<string>& ignore)
        {
            unordered_set<string> set;
            for (auto it = valves.begin(); it != valves.end(); it++)
            {
                if (it->second->rate > 0 && ignore.find(it->first) == ignore.end())
                    set.insert(it->first);
            }
            return set;
        }
    };

    Data data;

    class Progress
    {
    public:
        string path;
        long minute;
        long released;
        long increment = 0;
        string location;
        unordered_set<string> open;
        unordered_set<string> openable;

        Progress(long start_minute, long distance, string p, string dest, long initial_released, const unordered_set<string>& o, const unordered_set<string>& o2)
            : minute(start_minute)
            , released(initial_released)
            , location(dest)
            , open(o)
            , openable(o2)
        {
            path = p.size() == 0 ? dest : (p + " - " + dest);
            // If we haven't opened the dest valve yet, then plan one more minute to do that
            if (data.valves.at(dest)->rate > 0 && o.find(dest) == o.end())
                distance++;
            released += Releasing() * distance;  // releasing doesn't include the dest valve yet
            open.insert(dest);  // Open after travelling
            openable.erase(dest);
            minute += distance;
            increment = Releasing();
        }

        Progress() = default;
        Progress(const Progress& p) = default;

        const Progress& operator=(const Progress& p)
        {
            path = p.path;
            minute = p.minute;
            released = p.released;
            location = p.location;
            open = p.open;
            openable = p.openable;
            increment = p.increment;
            return *this;
        }

        long Releasing() const
        {
            long pressure = 0;
            for (auto it = open.begin(); it != open.end(); it++)
                pressure += data.valves.at(*it)->rate;
            return pressure;
        }

        bool operator==(const Progress& p) const
        {
            return minute == p.minute
                && released == p.released
                && location == p.location
                && open.size() == p.open.size()
                && Releasing() == p.Releasing();
        }

        bool operator<(const Progress& p) const
        {
            // Prioritize earlier, with more released
            if (minute != p.minute)
                return minute > p.minute;
            if (released != p.released)
                return released < p.released;
            if (location != p.location)
                return location < p.location;
            if (open.size() != p.open.size())
                return open.size() < p.open.size();
            return Releasing() < p.Releasing();
        }

        vector<Progress> Options(long remaining_minutes)
        {
            vector<Progress> options;
            Valve* v = data.valves.at(location);
            for (auto it = v->tunnels.begin(); it != v->tunnels.end(); it++)
            {
                if (it->second < remaining_minutes
                    && openable.find(it->first) != openable.end())
                {
                    Progress p(minute, it->second, path, it->first, released, open, openable);
                    options.push_back(p);
                }
            }
            if (options.size() == 0 && remaining_minutes > 0)
            {
                // Fast forward to the end, just sitting here
                Progress p(minute, remaining_minutes, path, location, released, open, openable);
                options.push_back(p);
            }
            return options;
        }
    };

    long Part1()
    {
        data.Init();

        unordered_set<string> open;
        unordered_set<string> openable = data.InterestingValves(open);
        priority_queue<Progress> queue;
        Progress start(0, 0, "", "AA", 0, open, openable);
        queue.push(start);

        long best = 0;
        long prev_min = 0;
        while (queue.size() > 0)
        {
            Progress p = queue.top();
            queue.pop();
            if (p.minute > prev_min)
                cout << "Minute " << (prev_min = p.minute) << " : queue is " << queue.size() << "\n";
            if (p.released > best)
                best = p.released;
            if (p.minute == 30)
            {
                cout << p.path;
                break;
            }

            auto next = p.Options(30 - p.minute);
            for (auto it = next.begin(); it != next.end(); it++)
                queue.push(*it);
        }
        return best;
    }

    long Part2()
    {
        data.Init();

        unordered_set<string> open;
        unordered_set<string> openable = data.InterestingValves(open);
        priority_queue<Progress> queue;
        Progress start(0, 0, "", "AA", 0, open, openable);
        queue.push(start);

        long best = 0;
        long prev_min = 0;
        while (queue.size() > 0)
        {
            Progress p = queue.top();
            queue.pop();
            if (p.increment >= ((data.max_flow + 1) / 2))
            {
                if (p.path == "AA - DD - HH - EE")
                {
                    cout << "Human: " << p.path << " (" << p.increment << " @" << p.minute << ")\n";
                }
                priority_queue<Progress> elephant;
                Progress eStart(0, 0, "", "AA", 0, open, p.openable);
                elephant.push(eStart);

                long prev_minE = 0;
                while (elephant.size() > 0)
                {
                    Progress e = elephant.top();
                    elephant.pop();
                    //if (e.minute > prev_minE)
                    //    cout << "Elephant minute " << (prev_minE = e.minute) << " : queue is " << elephant.size() << "\n";
                    if (e.openable.size() == 0)
                    {
                        long total = p.released + (26 - p.minute) * p.increment
                            + e.released + (26 - e.minute) * e.increment;
                        if (total > best)
                        {
                            best = total;
                            cout << best << " = Human: " << p.path << " (" << p.increment << " @" << p.minute
                                << "); Elephant: " << e.path << " (" << e.increment << " @" << e.minute << ")\n";
                        }
                        break;
                    }

                    auto next = e.Options(26 - e.minute);
                    for (auto it = next.begin(); it != next.end(); it++)
                        elephant.push(*it);
                    if (next.size() == 0)
                    {
                        long total = p.released + (26 - p.minute) * p.increment
                            + e.released + (26 - e.minute) * e.increment;
                        if (total > best)
                        {
                            best = total;
                            cout << best << " = Human: " << p.path << " (" << p.increment << " @" << p.minute
                                << "); Elephant: " << e.path << " (" << e.increment << " @" << e.minute << ")\n";
                        }
                    }
                }
                
                if (p.increment > (data.max_flow * 2) / 3)
                    continue;  // Don't continue deeper down the p branch
            }
            if (p.minute > prev_min)
                cout << "Human minute " << (prev_min = p.minute) << " : queue is " << queue.size() << "\n";
            if (p.minute >= 26)
                break;

            auto next = p.Options(26 - p.minute);
            for (auto it = next.begin(); it != next.end(); it++)
                queue.push(*it);
        }
        return best;
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