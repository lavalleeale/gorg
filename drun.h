#ifndef __drun_h__
#define __drun_h__
#include "plugin.h"
#include "match.h"
#include <filesystem>
class DrunMatch : public Match
{
public:
    DrunMatch(std::filesystem::path path) : path(path) {};
    std::string getDisplay() { return path.filename().string(); };
    void run() { system(path.string().c_str()); };
    float getRelevance(std::string input);
    ~DrunMatch() {};

private:
    std::filesystem::path path;
};
class Drun : public Plugin
{
public:
    float matchChance(std::string input);
    std::vector<Match *> getMatches(std::string input);
    ~Drun() {};
};
#endif