#include "CommandHandler.h"
#include <regex>

std::vector<std::string> CommandHandler::splitArgs(const std::string &input) {
    std::vector<std::string> tokens;

    //Regex to match words or quoted strings
    std::regex rgx(R"((\"[^\"]+\"|\S+))"); // command \"arg1 with spaces\" arg2 \"arg3\"
    auto wordsBegin = std::sregex_iterator(input.begin(), input.end(), rgx);
    auto wordsEnd = std::sregex_iterator();

    for(auto it = wordsBegin; it != wordsEnd; ++it) {
        std::string token = it->str();
        //Remove quotes
        if(token.size() >= 2 && token.front() == '\"' && token.back() == '\"') {
            token = token.substr(1, token.size() - 2);
        }
        tokens.push_back(token);
    }

    return tokens;
}