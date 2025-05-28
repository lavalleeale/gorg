#ifndef __match_h__
#define __match_h__

// C++ Standard Library
#include <string>

// GTK/GLib Libraries
#include <gtkmm/widget.h>

enum RunResult
{
    CLOSE,
    CONTINUE,
    TAKEOVER
};
class Match
{
public:
    virtual std::string getDisplay() const = 0;
    virtual std::string getIcon() const { return ""; };
    virtual RunResult run() = 0;
    virtual double getRelevance(const std::string &input) const = 0;
    virtual Gtk::Widget *getWidget() { return nullptr; };
    virtual ~Match() {};
};
#endif