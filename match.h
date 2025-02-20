#include <string>
#ifndef __match_h__
#define __match_h__
class Match
{
public:
    virtual std::string getDisplay() = 0;
    virtual void run() = 0;
    virtual float getRelevance(std::string input) = 0;
};
#endif