#ifndef __chat_h__
#define __chat_h__
#include "plugin.h"
#include <nlohmann/json.hpp>
#include "match.h"
#include <gtkmm/label.h>
#include <glibmm/dispatcher.h>
class ChatMatch : public Match
{
public:
    ChatMatch(std::string input);
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return &label; };
    ~ChatMatch() {};

private:
    nlohmann::json pluginSettings;
    std::string input;
    std::string responseText;
    Gtk::Label label;
    Glib::Dispatcher signal_update_label;
    static size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
};
class Chat : public Plugin
{
public:
    std::vector<Match *> getMatches(std::string input) { return {new ChatMatch(input)}; };
    ~Chat() {};

private:
};
#endif