#pragma once
#include <sstream>
#include <string>
#include <unordered_map>
#include "TokenClass.hpp"
#include "TokenPosition.hpp"

class Tokenizer
{
public:
    /**
     * Get token from string stream
     *
     * @param      stringStream   The string stream to get token from
     * @param      tokenClass     The class of identified token
     * @param      tokenPosition  The position of identified token
     *
     * @return     The string representation of a token
     */
    static std::string getToken(std::stringstream &stringStream,
                                TokenClass &tokenClass,
                                TokenPosition &tokenPosition);
    /**
     * Fix up type - replace known bad types with known good
     *
     * @param      typeName      String representation of type
     * @param      replacements  Additional string replacements
     *
     * @return     Fixed type name
     */
    static std::string fixupType(const std::string &typeName,
        const std::unordered_map<std::string, std::string> &replacements);

    /**
     * Add a space after string if there are none of them.
     *
     * @param      str   String object
     */
    static void addSpace(std::string &str);
};