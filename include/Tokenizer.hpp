#pragma once
#include <sstream>
#include <string>
#include "TokenClass.hpp"
#include "TokenPosition.hpp"

class Tokenizer
{
public:
    static std::string getToken(std::stringstream &ss,
                                TokenClass &cls,
                                TokenPosition &tp);
    static std::string fixupToken(const std::string &s);
    static void addSpace(std::string &s);
};