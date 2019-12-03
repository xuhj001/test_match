#include <string>
#include <vector>
#include <atomic>
#include <iostream>
#include "matcher.h"

std::atomic<ACMatcher*> g_word_matcher;

int main()
{
	std::string fill = "**";
//    std::vector<std::string> pattern = {"ABC"};
    std::vector<std::string> pattern = {"ab"};
    std::string text = "Abc 123 abc 456 Def";
    ACMatcher* mc = new ACMatcher;
    mc->Build(pattern, ACMatcher::kHasWord);
    g_word_matcher.store(mc);

    auto matcher = g_word_matcher.load(std::memory_order_relaxed);
    if (matcher != nullptr) {
      std::string pattern;
      if (matcher->MatchAndReplace(text, &pattern, fill)) {
        std::cout<<"matched "<<" pattern: "<<pattern << " text: "<<text <<std::endl;
      }
    }

	return 0;
}

