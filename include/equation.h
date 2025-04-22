#ifndef __equation_h__
#define __equation_h__
#include "plugin.h"
#include "match.h"
#include <exprtk.hpp>
#include <gtkmm/widget.h>
#include <nlohmann/json.hpp>
#include <settings.h>
class EquationMatch : public Match
{
public:
    EquationMatch(const std::string &input, float result) : input(input), result(result) {};
    std::string getDisplay() const override;
    RunResult run() override { return CONTINUE; };
    double getRelevance(const std::string &input) const override;
    ~EquationMatch() override {};

private:
    std::string input;
    float result;
    nlohmann::json pluginSettings = getPluginSettings("equation");
};
class Equation : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    ~Equation() override {};
};
#endif