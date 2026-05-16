#include <cassert>
#include <cmath>
#include <iostream>

#include <equation.h>
#include <stringMatch.h>
#include <web.h>

static void test_fuzzy_matching()
{
    assert(hasAllChars("ff", "Firefox"));
    assert(hasAllChars("fx", "Firefox"));
    assert(!hasAllChars("fz", "Firefox"));
    assert(fuzzyMatchScore("fire", "Firefox") > fuzzyMatchScore("fx", "Firefox"));
    assert(fuzzyMatchScore("", "Firefox") == 0.0);
}

static void test_equation_match()
{
    Equation equation;
    equation.setSettings(nlohmann::json::object());

    auto matches = equation.getMatches("1 + 2 * 3");
    assert(matches.size() == 1);
    assert(matches.front()->getDisplay() == "1 + 2 * 3 = 7");

    matches = equation.getMatches("not an equation");
    assert(matches.empty());
}

static void test_url_encoding()
{
    assert(url_encode("hello world") == "hello%20world");
    assert(url_encode("a+b=c") == "a%2bb%3dc");
    assert(url_encode("safe-_.~") == "safe-_.~");
}

int main()
{
    test_fuzzy_matching();
    test_equation_match();
    test_url_encoding();
    std::cout << "core tests passed\n";
    return 0;
}
