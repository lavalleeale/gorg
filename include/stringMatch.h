#ifndef __stringmatch_h__
#define __stringmatch_h__
#include <string>
#include <cctype>
#include <algorithm>
#include <cmath>

#define START_BONUS 10
#define CONSECUTIVE_BONUS 10
#define EXTRA_CHAR_PENALTY 2

inline double fuzzyMatchScore(const std::string &query, const std::string &candidate)
{
    if (query.empty())
    {
        return 0.0;
    }

    int score = 0;
    int consecutiveBonus = 0;
    size_t queryIndex = 0;

    // Iterate through each character in the candidate string
    for (size_t i = 0; i < candidate.size() && queryIndex < query.size(); i++)
    {
        // Compare characters case-insensitively
        if (std::tolower(query[queryIndex]) == std::tolower(candidate[i]))
        {
            if (i == 0 || candidate[i - 1] == ' ')
            {
                score += START_BONUS; // Word-start bonus
            }
            consecutiveBonus++; // Increment bonus since this is a match
            score += CONSECUTIVE_BONUS * consecutiveBonus * consecutiveBonus;
            queryIndex++;
        }
        else
        {
            consecutiveBonus = 0; // Reset bonus if not a consecutive match
        }
    }

    // Penalize extra characters: subtract 2 points for each extra character in candidate.
    if (queryIndex == query.size())
    {
        int extraChars = candidate.size() - query.size();
        score -= EXTRA_CHAR_PENALTY * extraChars;
    }

    // If not all characters in the query were matched, indicate failure with 0.
    if (queryIndex != query.size())
    {
        return 0.0;
    }

    // Compute maximum possible score for a contiguous match of the query.
    // For first character: bonus = 1 yields 10 (word-start bonus) + 10 * (1^2) = 20.
    // For subsequent characters, best-case score is 10 * (i^2) for i = 2..query.size().
    int Q = query.size();
    double maxScore = START_BONUS + CONSECUTIVE_BONUS;
    for (int i = 2; i <= Q; i++)
    {
        maxScore += CONSECUTIVE_BONUS * i * i;
    }

    // Normalize score between 0 and 1.
    double normalized = static_cast<double>(score) / maxScore;
    normalized = std::clamp(normalized, 0.0, 1.0);
    return normalized;
}

inline bool hasAllChars(const std::string &query, const std::string &candidate)
{
    if (query.empty())
    {
        return true;
    }
    std::string lowerQuery = query;
    std::string lowerCandidate = candidate;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    std::transform(lowerCandidate.begin(), lowerCandidate.end(), lowerCandidate.begin(), ::tolower);

    // Store the iterator to the first character of the candidate string
    std::string::iterator candidateIndex = lowerCandidate.begin();
    for (auto c : lowerQuery)
    {
        // Find the first occurrence of the character that has not been used yet
        candidateIndex = std::find(candidateIndex, lowerCandidate.end(), c);
        // If the character is not found, return false
        if (candidateIndex == lowerCandidate.end())
        {
            return false;
        }
        // Move the iterator to the next character for the next iteration
        candidateIndex++;
    }
    return true;
}
#endif
