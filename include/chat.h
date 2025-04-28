#ifndef __chat_h__
#define __chat_h__

// GTK/GLib Libraries
#include <glibmm/dispatcher.h>
#include <gtkmm/label.h>

// Local Headers
#include <match.h>
#include <plugin.h>

class ChatMatch : public Match
{
public:
    ChatMatch(const std::string &input);
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    Gtk::Widget *getWidget() override { return &label; };
    ~ChatMatch() override {};

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
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "chat"; };
    ~Chat() override {};

private:
};
#endif