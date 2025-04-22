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
    EquationMatch(std::string input, float result) : input(input), result(result), pluginSettings(getPluginSettings("equation")) {};
    std::string getDisplay();
    RunResult run() { return CONTINUE; };
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~EquationMatch() {};

private:
    std::string input;
    float result;
    nlohmann::json pluginSettings;
};
class Equation : public Plugin
{
public:
    std::vector<Match *> getMatches(std::string input);
    ~Equation() {};

private:
    exprtk::parser<double> parser;
    exprtk::expression<double> expression;
};
#endif