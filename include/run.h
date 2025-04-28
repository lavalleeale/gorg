#ifndef __run_h__
#define __run_h__

// External Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <settings.h>

class RunMatch : public Match
{
public:
    RunMatch(const std::string &exec, const std::string &name, double relevance) : exec(exec), name(name), relevance(relevance) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~RunMatch() override {};

private:
    std::string exec;
    std::string name;
    double relevance;
};
class Run : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "run"; };
    ~Run() override {};
};
#endif