#ifndef __levenshtein_h__
#define __levenshtein_h__
#include <string>
double fuzzyMatchScore(const std::string &query, const std::string &candidate);
#endif