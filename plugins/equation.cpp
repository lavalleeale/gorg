// External Libraries
#include <exprtk.hpp>

// Project Headers
#include <equation.h>

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

std::vector<Match *> Equation::getMatches(const std::string &input) const
{
    if (currentMatch->setInput(input))
    {
        return {currentMatch};
    }
    else
    {
        return {};
    }
}

std::string EquationMatch::getDisplay() const
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

bool EquationMatch::setInput(const std::string &input)
{
    if (parser.compile(input, expression))
    {
        this->input = input;
        this->result = expression.value();
        return true;
    }
    else
    {
        this->input = "";
        this->result = 0.0f; // Reset result if parsing fails
        return false;
    }
}

void Equation::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
    currentMatch = new EquationMatch("", 0, pluginSettings.value("relevance", 1));
}