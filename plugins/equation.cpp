#include "equation.h"
#include <exprtk.hpp>
#include <iomanip>

// Trim from the left
std::string ltrim(std::string str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    return str;
}

// Trim from the right
std::string rtrim(std::string str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch)
                           { return !std::isspace(ch); })
                  .base(),
              str.end());
    return str;
}

// Trim from both ends
std::string trim(std::string str)
{
    return ltrim(rtrim(str));
}

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
    // only show last expression in input
    std::string lhs = trim(input);
    if (lhs.back() == ';')
    {
        lhs.pop_back();
    }
    return lhs.substr(lhs.find_last_of(';') + 1) + " = " + oss.str();
}

double EquationMatch::getRelevance(std::string input)
{
    return 1.0;
}
