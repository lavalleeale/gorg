#ifndef __sample_h__
#define __sample_h__
#include <plugin.h>
#include <nlohmann/json.hpp>
#include <settings.h>

class SampleMatch : public Match
{
public:
    SampleMatch(const std::string &value) : value(value) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~SampleMatch() override {};

private:
    std::string value;
};

class SamplePlugin : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "sample"; };

private:
};
extern "C" Plugin *create();
#endif