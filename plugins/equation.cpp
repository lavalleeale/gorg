#include "equation.h"
#include <exprtk.hpp>

float Equation::matchChance(std::string input)
{
    return parser.compile(input, expression) ? 1 : 0;
}

std::vector<Match *> Equation::getMatches(std::string input)
{
    if (parser.compile(input, expression))
    {
        return std::vector<Match *>({new EquationMatch(input, expression.value())});
    }
    else
    {
        return std::vector<Match *>();
    }
}

std::string EquationMatch::getDisplay()
{
    return input + "=" + std::to_string(result);
}

double EquationMatch::getRelevance(std::string input)
{
    return 1.0;
}
