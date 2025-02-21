#include <string>
#ifndef __levenshtein_h__
#define __levenshtein_h__
double fuzzyMatchScore(const std::string &query, const std::string &candidate);
#endif