#include "Tokenizer.hpp"

std::string
Tokenizer::getToken(std::stringstream &stringStream,
                    TokenClass &       tokenClass,
                    TokenPosition &    tokenPosition)
{
    std::string s;
    char        c;

    if (stringStream.eof() || stringStream.rdbuf()->in_avail() == 0)
    {
        tokenClass = TokenClass::Unknown;
        return "";
    }

    stringStream >> c;
    if (c == '\0')
    {
        tokenClass = TokenClass::Unknown;
        return "";
    }

    tokenClass = TokenClass::Type;
    tokenPosition = TokenPosition::Middle;
    switch (c)
    {
        case 'v':
            return "void";
        case 'b':
            return "bool";
        case 'c':
            return "char";
        case 's':
            return "short";
        case 'i':
            return "int";
        case 'h':
            return "half";
        case 'f':
            return "float";
        case 'd':
            return "double";
        case 'z':
            return "size_t";
        case 'w':
            return "wchar_t";
        case 'F':
            return "CFString";
        case 'G':
        case 'H':
            return "";
        case 'M':
            return "struct objc_super";
        case 'a':
            return "__builtin_va_list";
        case 'A':
            return "__builtin_va_list &";
        case 'V':
        case 'E':
            return "";
        case 'X':
        case 'Y':
            return "ptrdiff_t";
        case 'P':
            return "FILE";
        case 'J':
            return "jmp_buf";
        case 'S':
        {
            char c2;

            c2 = stringStream.peek();
            if (c2 == 'J')
            {
                /* Spoils next token */
                return "sigjmp_buf";
            }
            else
            {
                /* it is not sigjmp_buf */
                break;
            }
            break;
        }
        case 'K':
            return "ucontext_t";
        case 'p':
            return "pid_t";
        case '.':
            return "...";
        default:
            break;
    }

    tokenClass = TokenClass::PreModifier;
    tokenPosition = TokenPosition::Before;
    switch (c)
    {
        case 'L':
        {
            std::string s;

            s += c;

            while (true)
            {
                int next = stringStream.peek();

                if (next != 'L')
                    break;

                stringStream >> c;
                s += c;
            }

            if (s == "L")
            {
                return "long";
            }
            else if (s == "LL")
            {
                return "long long";
            }
            else if (s == "LLL")
            {
                /* obviously wrong, but let's fix it later */
                return "long long long";
            }
            break;
        }
        case 'Z':
            /* basically int32_t */
            return "";
        case 'W':
            /* basically int64_t */
            return "";
        case 'N':
            return "";
        case 'O':
            return "long long";
        case 'S':
            return "signed";
        case 'U':
            return "unsigned";
        case 'I':
            /* check next */
            return getToken(stringStream, tokenClass, tokenPosition);
        default:
            break;
    }

    tokenClass = TokenClass::PostModifier;
    tokenPosition = TokenPosition::After;
    switch (c)
    {
        case '*':
            return "*";
        case '&':
            return "&";
        case 'R':
            return "restrict";
        /*
         * Basically pre modifiers
         */
        case 'C':
            tokenPosition = TokenPosition::Before;
            return "const";
        case 'D':
            tokenPosition = TokenPosition::Before;
            return "volatile";
        default:
            break;
    }

    throw std::exception();
}

std::string
Tokenizer::fixupType(const std::string &s)
{
    if (s == "long long double")
        return "__float128";
    if (s == "long long long int")
        return "__int128_t";
    return s;
}

void
Tokenizer::addSpace(std::string &s)
{
    if (s.size() == 0)
        return;

    if (s.back() != ' ')
        s += " ";
}
