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
namespace Day16b
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
        int working_valves;

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

                for (auto it = valves.begin(); it != valves.end(); it++)
                {
                    it->second->RemoveZeros(valves);
                }
                working_valves = valves["AA"]->tunnels.size();
                if (valves["AA"]->rate > 0)
                    working_valves++;
            }
        }
    };

    Data data;

    class Progress
    {
    public:
        string path;
        long minute;
        long released;
        string location;
        unordered_set<string> open;

        Progress(long start_minute, long distance, string p, string dest, long initial_released, unordered_set<string> o)
            : minute(start_minute)
            , released(initial_released)
            , location(dest)
            , open(o)
        {
            path = p.size() == 0 ? dest : (p + " - " + dest);
            // If we haven't opened the dest valve yet, then plan one more minute to do that
            if (data.valves.at(dest)->rate > 0 && o.find(dest) == o.end())
                distance++;
            released += Releasing() * distance;  // releasing doesn't include the dest valve yet
            open.insert(dest);  // Open after travelling
            minute += distance;
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
                    && open.find(it->first) == open.end())
                {
                    Progress p(minute, it->second, path, it->first, released, open);
                    options.push_back(p);
                }
            }
            if (options.size() == 0 && remaining_minutes > 0)
            {
                // Fast forward to the end, just sitting here
                Progress p(minute, remaining_minutes, path, location, released, open);
                options.push_back(p);
            }
            return options;
        }
    };

    long Part1()
    {
        data.Init();

        unordered_set<string> open;
        priority_queue<Progress> queue;
        Progress start(0, 0, "", "AA", 0, open);
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
                break;

            auto next = p.Options(30 - p.minute);
            for (auto it = next.begin(); it != next.end(); it++)
                queue.push(*it);
        }
        return best;
    }

    class Partial
    {
    public:
        string path;
        string dest;
        int distance;  // includes 1 for opening

        Partial(string start)
            : path(start)
            , dest(start)
            , distance(0)
        {
        }

        Partial(string p, string d, int dist)
            : path(p)
            , dest(d)
            , distance(dist + 1)
        {
        }

        Partial(const Partial& p) = default;
        const Partial& operator=(const Partial& p)
        {
            path = p.path;
            dest = p.dest;
            distance = p.distance;
            return *this;
        }

        bool operator==(const Partial& p) const
        {
            return path == p.path
                && dest == p.dest
                && distance == p.distance;
        }

        bool operator<(const Partial& p) const
        {
            // Prioritize less distance
            if (distance != p.distance)
                return distance > p.distance;
            if (dest != p.dest)
                return dest > p.dest;
            if (path != p.path)
                return path > p.path;
            return false;
        }
    };

    class Progress2
    {
    public:
        long minute;
        long released;
        long increment;
        Partial myself, elephant;
        unordered_set<string> open;

        Progress2(string start, unordered_set<string> o)
            : minute(0)
            , released(0)
            , increment(0)
            , myself(start)
            , elephant(start)
            , open(o)
        {
        }

        Progress2() = default;
        Progress2(const Progress2& p) = default;

        const Progress2& operator=(const Progress2& p)
        {
            minute = p.minute;
            released = p.released;
            increment = p.increment;
            myself = p.myself;
            elephant = p.elephant;
            open = p.open;
            return *this;
        }

        long Releasing() const
        {
            long pressure = 0;
            for (auto it = open.begin(); it != open.end(); it++)
                pressure += data.valves.at(*it)->rate;
            return pressure;
        }

        bool operator==(const Progress2& p) const
        {
            return minute == p.minute
                && released == p.released
                && myself == p.myself
                && elephant == p.elephant
                && open.size() == p.open.size()
                && increment == p.increment;
        }

        bool operator<(const Progress2& p) const
        {
            // Prioritize earlier, with more released
            if (minute != p.minute)
                return minute > p.minute;
            if (released != p.released)
                return released < p.released;
            if (open.size() != p.open.size())
                return open.size() < p.open.size();
            if (increment != p.increment)
                return increment < p.increment;
            if (!(myself == p.myself))
                return myself < p.myself;
            if (!(elephant == p.elephant))
                return elephant < p.elephant;
            return false;
        }

        void SetNext(const Partial& m, const Partial& e)
        {
            int steps = min(m.distance, e.distance);
            minute += steps;
            released += steps * increment;
            myself = m;
            elephant = e;

            if (myself.distance <= elephant.distance)
            {
                open.insert(m.dest);
                myself.path += " - " + myself.dest;
                myself.path += "(" + to_string(minute) + ")";
                elephant.distance -= myself.distance;
                myself.distance = 0;
            }
            if (elephant.distance <= myself.distance)
            {
                open.insert(e.dest);
                elephant.path += " - " + elephant.dest;
                elephant.path += "(" + to_string(minute) + ")";
                myself.distance -= elephant.distance;
                elephant.distance = 0;
            }
            increment = Releasing();
        }

        long ScoreAt(long future) const
        {
            long plan = minute + max(myself.distance, elephant.distance);
            if (plan >= future)
                return released + increment * (future - minute);

            long increase = 0;
            if (myself.distance > 0)
                increase += data.valves.at(myself.dest)->rate;
            else if (elephant.distance > 0)
                increase += data.valves.at(elephant.dest)->rate;
            return released + increment * (future - minute)
                + increase * (future - plan);
        }

        bool IsMirror(const Progress2& p, bool& isBetter) const
        {
            if (myself.dest == p.elephant.dest && elephant.dest == p.myself.dest)
            {
                long future = max(p.minute + max(p.myself.distance, p.elephant.distance),
                    minute + max(myself.distance, elephant.distance));
                long us = ScoreAt(future);
                long them = p.ScoreAt(future);
                isBetter = us >= them;
                return true;
            }
            return false;
        }

        vector<Progress2> Options(long remaining_minutes)
        {
            vector<Progress2> options;
            // Three possibilities: I have arrived, while elephant is still traveling
            // Elephant has arrived, while I am still traveling
            // Simultaneously arrived

            vector<Partial> myNext, elNext;
            if (myself.distance == 0)
            {
                Valve* v = data.valves.at(myself.dest);
                for (auto it = v->tunnels.begin(); it != v->tunnels.end(); it++)
                {
                    if (it->second < remaining_minutes
                        && open.find(it->first) == open.end())
                    {
                        myNext.push_back(Partial(myself.path, it->first, it->second));
                    }
                }
                if (myNext.size() == 0)
                    myNext.push_back(Partial(myself.path, myself.dest, remaining_minutes - 1));
            }
            else
                myNext.push_back(myself);
            if (elephant.distance == 0)
            {
                Valve* v = data.valves.at(elephant.dest);
                for (auto it = v->tunnels.begin(); it != v->tunnels.end(); it++)
                {
                    if (it->second < remaining_minutes
                        && open.find(it->first) == open.end())
                    {
                        elNext.push_back(Partial(elephant.path, it->first, it->second));
                    }
                }
                if (elNext.size() == 0)
                    elNext.push_back(Partial(elephant.path, elephant.dest, remaining_minutes - 1));
            }
            else
                elNext.push_back(elephant);

            for (auto mit = myNext.begin(); mit != myNext.end(); mit++)
            {
                for (auto eit = elNext.begin(); eit != elNext.end(); eit++)
                {
                    if (mit->dest == eit->dest && open.size() < data.working_valves - 1)
                        continue;  // It'll never be optimal to send both of us to the same dest, except for the last
                    Progress2 p(*this);
                    p.SetNext(*mit, *eit);
                    options.push_back(p);
                }
            }

            // Remove mirror images
            for (int i = 0; i < options.size(); i++)
            {
                for (int j = i + 1; j < options.size(); j++)
                {
                    bool isBetter;
                    if (options[i].IsMirror(options[j], isBetter))
                    {
                        if (isBetter)
                        {
                            auto del = options.begin() + j;
                            options.erase(del);
                            break;
                        }
                        else
                        {
                            auto del = options.begin() + i;
                            options.erase(del);
                            i--;
                            break;
                        }
                    }
                }
            }

            return options;
        }
    };

    long Part2()
    {
        data.Init();

        unordered_set<string> open;
        priority_queue<Progress2> queue;
        Progress2 start("AA", open);
        queue.push(start);

        long best = 0;
        long prev_min = 0;
        while (queue.size() > 0)
        {
            Progress2 p = queue.top();
            queue.pop();
            if (p.minute > prev_min)
                cout << "Minute " << (prev_min = p.minute) << " : queue is " << queue.size() << "\n";
            if (p.released > best)
                best = p.released;
            if (p.minute >= 26)
            {
                cout << p.myself.path << "; " << p.elephant.path << "\n";
                break;
            }

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