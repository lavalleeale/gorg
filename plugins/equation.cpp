#include "equation.h"
#include <exprtk.hpp>
#include <iomanip>

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
    std::ostringstream oss;
    // Format with 6 decimal places, then trim unnecessary zeros.
    oss << std::defaultfloat << std::setprecision(std::numeric_limits<float>::max_digits10) << result;
    return input + "=" + oss.str();
}

double EquationMatch::getRelevance(std::string input)
{
    return 1.0;
}
