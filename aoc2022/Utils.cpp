#include "Utils.h"

using namespace std;

string DayFromNamespace(const char* funcsig)
{
    string s(funcsig);
    auto i = s.find("Day");
    s = s.substr(i);
    i = s.find("::");
    s = s.substr(0, i);
    return s;
}

string FileFromNamespace(const char* funcsig)
{
    string s(funcsig);
    auto i = s.find("Day");
    s = s.substr(i);
    i = s.find("::");
    s = s.substr(0, i);
    return "Data\\" + s + ".txt";
}
