#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <vector>
#include "BuiltinizerMode.hpp"
#include "TokenClass.hpp"
#include "TokenPosition.hpp"
#include "LibraryFunction.hpp"
#include "Tokenizer.hpp"


#define BUILTIN(ID, TYPE, ATTRS) LibraryFunction(#ID, TYPE, ATTRS),

#if defined(BUILTIN) && !defined(LIBBUILTIN)
#  define LIBBUILTIN(ID, TYPE, ATTRS, HEADER, BUILTIN_LANG) BUILTIN(ID, TYPE, ATTRS)
#endif

#if defined(BUILTIN) && !defined(LANGBUILTIN)
#  define LANGBUILTIN(ID, TYPE, ATTRS, BUILTIN_LANG) BUILTIN(ID, TYPE, ATTRS)
#endif

LibraryFunction funcs[] = {
    #include "Builtins.def"
};

std::string
reconstructSignature(LibraryFunction *func)
{
    std::string   signature;
    int           mode = 0;
    TokenClass    cls;
    TokenPosition tp;
    std::string   s;
    std::vector<std::string> types;
    std::stringstream        ss(func->getType());

    while (true)
    {
        int  newMode;
        auto tok = Tokenizer::getToken(ss, cls, tp);

        if (cls == TokenClass::Unknown)
            break;

        /*
         * Start new complete type token on start, when lower priority token
         * appears (with respect to real values of TokenClass) or when mode
         * doesn't change, yet another modifier appears
         */
        newMode = (int)cls;
        if (mode == 0 ||
            (newMode <= mode &&
            !(newMode == mode &&
              cls == TokenClass::PreModifier ||
              cls == TokenClass::PostModifier)))
        {
            if (s != "")
                types.push_back(s);
            s = "";
        }

        if (s == "")
        {
            /* Token is empty, just fill it up */
            s = tok;
        }
        else if (tp == TokenPosition::Before)
        {
            /* Put token before the current one */
            Tokenizer::addSpace(tok);
            s = tok + s;
        }
        else
        {
            /* Put token after the current one */
            Tokenizer::addSpace(s);
            s += tok;
        }
        mode = newMode;
    }

    /* Fill the unprocessed type */
    if (s != "")
        types.push_back(s);

    /* Rebuild signature */
    if (types.size() > 0)
    {
        signature += Tokenizer::fixupToken(types[0]) + " " + func->getId() +
            "(";

        for (int i = 1; i < types.size(); ++i)
        {
            if (i != 1)
                signature += ", ";
            signature += Tokenizer::fixupToken(types[i]);
        }
        signature += ");";
    }
    return signature;
}

int main(int argc, const char *argv[])
{
    std::string s;
    size_t      sigs = 0;
    bool        ignoreHalf = false;
    bool        constExpr = false;
    BuiltinizerMode mode = BuiltinizerMode::Signatures;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = std::string(argv[i]);

        if (arg == "--list")
            mode = BuiltinizerMode::List;
        else if (arg == "--constexpr")
            constExpr = true;
        else if (arg == "--ignore-half")
            ignoreHalf = true;
    }

    for (int i = 0; i < sizeof(funcs) / sizeof(*funcs); ++i)
    {
        auto f = &funcs[i];
        auto sig = reconstructSignature(f);
        bool hasHalf = sig.find("half") != std::string::npos;

        if (!ignoreHalf || !hasHalf)
        {
            bool isConst = f->getAttrs().find("c") != std::string::npos;

            if (!constExpr || isConst)
            {
                switch (mode)
                {
                    case BuiltinizerMode::Signatures:
                        if (constExpr)
                            std::cout << "constexpr ";
                        std::cout << sig << std::endl;
                        break;
                    case BuiltinizerMode::List:
                        if (sigs != 0)
                            std::cout << ",";
                        std::cout << f->getId();
                        break;
                }
                sigs++;
            }
        }
    }
    return 0;
}