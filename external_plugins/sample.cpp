#include <iostream>
#include "sample.h"

std::string SampleMatch::getDisplay() const
{
    return "Sample Match: " + input;
}

RunResult SampleMatch::run()
{
    std::cout << "Running Sample Match with input: " << input << std::endl;
    return RunResult::CLOSE;
}

double SampleMatch::getRelevance(const std::string &input) const
{
    // Simple relevance calculation based on input length
    return 1;
}

std::vector<Match *> SamplePlugin::getMatches(const std::string &input) const
{
    std::vector<Match *> matches;
    matches.push_back(new SampleMatch(input));
    return matches;
}

extern "C" Plugin *create()
{
    return new SamplePlugin();
}