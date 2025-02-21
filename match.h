#include <string>
#ifndef __match_h__
#define __match_h__
class Match
{
public:
    virtual std::string getDisplay() = 0;
    virtual bool run() = 0;
    virtual double getRelevance(std::string input) = 0;
    virtual ~Match() {};
};
#endif