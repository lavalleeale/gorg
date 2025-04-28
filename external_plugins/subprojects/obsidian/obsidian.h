#ifndef __obsidian_h__
#define __obsidian_h__
#include <plugin.h>
#include <filesystem>

class ObsidianMatch : public Match
{
public:
    ObsidianMatch(const std::string &value) : value(value) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~ObsidianMatch() override {};

private:
    std::filesystem::path value;
};
class ObsidianPlugin : public Plugin
{
public:
    void setSettings(const nlohmann::json &settings) override;
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "obsidian"; };

private:
    std::vector<std::filesystem::path> notes;
    // Obsidian specific settings
};

extern "C" Plugin *create();

#endif