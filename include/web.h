#ifndef __web_h__
#define __web_h__

#include <memory>

// External Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <settings.h>

std::string url_encode(const std::string &value);

class WebMatch : public Match
{
public:
    WebMatch(const std::string &input, double relevance, const std::string &searchUrl) : input(input), relevance(relevance), searchUrl(searchUrl) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    void updateInput(const std::string &input);
    ~WebMatch() override {};

private:
    std::string input;
    double relevance;
    std::string searchUrl;
};
class Web : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "web"; };
    void setSettings(const nlohmann::json &settings) override;
    ~Web() override {};

private:
    std::unique_ptr<WebMatch> currentMatch;
};
#endif
