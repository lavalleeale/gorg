
#ifndef __chat_h__
#define __chat_h__
#include "plugin.h"
#include "match.h"
#include <gtkmm/label.h>
class ChatMatch : public Match
{
public:
    ChatMatch(std::string input);
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return &label; };
    ~ChatMatch() {};
    void updateResponse(std::string response);

private:
    std::string input;
    Gtk::Label label;
};
class Chat : public Plugin
{
public:
    float matchChance(std::string input) { return 0.75; };
    std::vector<Match *> getMatches(std::string input) { return {new ChatMatch(input)}; };
    ~Chat() {};

private:
};
#endif