#ifndef __equation_h__
#define __equation_h__
#include "plugin.h"
#include "match.h"
#include <exprtk.hpp>
class EquationMatch : public Match
{
public:
    EquationMatch(std::string input, float result) : input(input), result(result) {};
    std::string getDisplay();
    bool run() { return false; };
    double getRelevance(std::string input);
    ~EquationMatch() {};

private:
    std::string input;
    float result;
};
class Equation : public Plugin
{
public:
    float matchChance(std::string input);
    std::vector<Match *> getMatches(std::string input);
    ~Equation() {};

private:
    exprtk::parser<float> parser;
    exprtk::expression<float> expression;
};
#endif