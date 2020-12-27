#pragma once
#include <string>

class LibraryFunction
{
public:
    LibraryFunction(const char *id, const char *type, const char *attrs)
    {
        _id = std::string(id);
        _type = std::string(type);
        _attrs = std::string(attrs);
    }

    virtual ~LibraryFunction()
    {
    }

    /**
     * @brief      Get the identifier of function
     *
     * @return     The identifier.
     */
    std::string getId() const
    {
        return _id;
    }

    /**
     * @brief      Get type as defined in Clang's Builtins.def
     *
     * @return     The type.
     */
    std::string getType() const
    {
        return _type;
    }

    /**
     * @brief      Get attributes as defined in Clang's Builtins.def
     *
     * @return     The attributes.
     */
    std::string getAttrs() const
    {
        return _attrs;
    }

private:
    std::string _id;
    std::string _type;
    std::string _attrs;
};
