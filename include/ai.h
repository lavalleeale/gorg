#ifndef __chat_h__
#define __chat_h__

// GTK/GLib Libraries
#include <glibmm/dispatcher.h>
#include <gtkmm/label.h>

// Local Headers
#include <match.h>
#include <plugin.h>

class AIMatch : public Match
{
public:
    AIMatch(const std::string &input, double relevance, int max_width, const std::string &url, const std::string &apiKey, const std::string &model);
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    Gtk::Widget *getWidget() override { return &label; };
    void updateInput(const std::string &input);
    ~AIMatch() override {};

private:
    std::string input;
    std::string responseText;
    Gtk::Label label;
    Glib::Dispatcher signal_update_label;
    static size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
    double relevance;
    std::string url, apiKey, model;
};
class AI : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "ai"; };
    void setSettings(const nlohmann::json &settings) override;
    ~AI() override;

private:
    AIMatch *currentMatch = nullptr;

private:
};
#endif