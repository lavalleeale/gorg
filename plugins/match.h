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
    virtual std::string getDisplay() = 0;
    virtual RunResult run() = 0;
    virtual double getRelevance(std::string input) = 0;
    virtual Gtk::Widget *getWidget() = 0;
    virtual ~Match() {};
};
#endif