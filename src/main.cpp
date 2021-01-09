#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "BuiltinizerMode.hpp"
#include "LibraryFunction.hpp"
#include "TokenClass.hpp"
#include "TokenPosition.hpp"
#include "Tokenizer.hpp"

struct Options
{
    Options()
    {
        ignoreHalf = false;
        constExpr = false;
        constExprMath = false;
        mode = BuiltinizerMode::Signatures;
        replacements = std::unordered_map<std::string, std::string>();
        ignoreFunctions = std::unordered_map<std::string, bool>();
        ignoreLibraryFunctions = false;
        forceFunctions = std::unordered_map<std::string, bool>();
    }

    bool                                         ignoreHalf;
    bool                                         constExpr;
    bool                                         constExprMath;
    BuiltinizerMode                              mode;
    std::unordered_map<std::string, std::string> replacements;
    std::unordered_map<std::string, bool>        ignoreFunctions;
    bool                                         ignoreLibraryFunctions;
    std::unordered_map<std::string, bool>        forceFunctions;
};

#define DEF_BUILTIN(ID, TYPE, ATTRS, META_TYPE) \
    LibraryFunction(#ID, TYPE, ATTRS, META_TYPE),

#if defined(DEF_BUILTIN)
#define BUILTIN(ID, TYPE, ATTRS) \
    DEF_BUILTIN(ID, TYPE, ATTRS, MetaType::Builtin)
#endif
#if defined(BUILTIN) && !defined(LIBBUILTIN)
#define LIBBUILTIN(ID, TYPE, ATTRS, HEADER, BUILTIN_LANG) \
    DEF_BUILTIN(ID, TYPE, ATTRS, MetaType::StandardLibrary)
#endif

#if defined(BUILTIN) && !defined(LANGBUILTIN)
#define LANGBUILTIN(ID, TYPE, ATTRS, BUILTIN_LANG) \
    DEF_BUILTIN(ID, TYPE, ATTRS, MetaType::LanguageBuiltin)
#endif

LibraryFunction funcs[] = {
#include "Builtins.def"
};


/**
 * Reconstruct signature from Clang's Builtin declaration
 *
 * @param      func  The library function pointer
 *
 * @return     String representation of function signature
 */
std::string
reconstructSignature(LibraryFunction *func, const Options &opts)
{
    std::string              signature;
    int                      mode = 0;
    TokenClass               cls;
    TokenPosition            tp;
    std::string              s;
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
               (cls == TokenClass::PreModifier ||
                cls == TokenClass::PostModifier))))
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
        signature += Tokenizer::fixupType(types[0], opts.replacements) + " " +
            func->getId() + "(";

        for (int i = 1; i < types.size(); ++i)
        {
            if (i != 1)
                signature += ", ";
            signature += Tokenizer::fixupType(types[i], opts.replacements);
        }
        signature += ");";
    }
    return signature;
}

int
main(int argc, const char *argv[])
{
    std::string s;
    size_t      sigs = 0;
    Options     opts;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = std::string(argv[i]);

        if (arg == "--list")
        {
            opts.mode = BuiltinizerMode::List;
        }
        else if (arg == "--constexpr")
        {
            opts.constExpr = true;
        }
        else if (arg == "--constexpr-math")
        {
            opts.constExprMath = true;
        }
        else if (arg == "--ignore-half")
        {
            opts.ignoreHalf = true;
        }
        else if (arg == "--replace")
        {
            if ((i + 1) < argc)
            {
                std::string replacement = std::string(argv[i + 1]);
                auto        delimPos = replacement.find(':');

                if (delimPos != std::string::npos)
                {
                    opts.replacements[replacement.substr(0, delimPos)] =
                        replacement.substr(delimPos + 1);
                }
                ++i;
            }
        }
        else if (arg == "--ignore")
        {
            if ((i + 1) < argc)
            {
                std::string fn = std::string(argv[i + 1]);

                opts.ignoreFunctions[fn] = true;
                ++i;
            }
        }
        else if (arg == "--ignore-lib")
        {
            opts.ignoreLibraryFunctions = true;
        }
        else if (arg == "--force")
        {
            if ((i + 1) < argc)
            {
                std::string fn = std::string(argv[i + 1]);

                opts.forceFunctions[fn] = true;
                ++i;
            }
        }
    }

    for (int i = 0; i < sizeof(funcs) / sizeof(*funcs); ++i)
    {
        auto        f = &funcs[i];
        std::string sig;
        bool        hasHalf;

        if (opts.ignoreFunctions[f->getId()])
            continue;

        if (opts.ignoreLibraryFunctions &&
            f->getMetaType() == MetaType::StandardLibrary)
            continue;

        sig = reconstructSignature(f, opts);
        hasHalf = sig.find("half") != std::string::npos;

        if (!opts.ignoreHalf || !hasHalf)
        {
            bool isConst = f->getAttrs().find("c") != std::string::npos ||
                (opts.constExprMath &&
                 f->getAttrs().find("e") != std::string::npos);
            bool forced = opts.forceFunctions.find(f->getId()) !=
                opts.forceFunctions.end();

            if (!opts.constExpr || isConst || forced)
            {
                switch (opts.mode)
                {
                    case BuiltinizerMode::Signatures:
                        if (opts.constExpr)
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