#include <string>
#include <gtkmm/widget.h>
#ifndef __match_h__
#define __match_h__

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
    virtual RunResult run() = 0;
    virtual double getRelevance(const std::string &input) const = 0;
    virtual Gtk::Widget *getWidget() { return nullptr; };
    virtual ~Match() {};
};
#endif