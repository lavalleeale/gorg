#ifndef __equation_h__
#define __equation_h__

// External Libraries
#include <exprtk.hpp>
#include <nlohmann/json.hpp>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <settings.h>

class EquationMatch : public Match
{
public:
    EquationMatch(const std::string &input, float result, double relevance) : input(input), result(result), relevance(relevance) {};
    std::string getDisplay() const override;
    RunResult run() override { return CONTINUE; };
    double getRelevance(const std::string &input) const override { return relevance; };
    ~EquationMatch() override {};

private:
    std::string input;
    float result;
    double relevance;
};
class Equation : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "equation"; };
    ~Equation() override {};
};
#endif