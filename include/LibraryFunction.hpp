#pragma once
#include <string>
#include "MetaType.hpp"

class LibraryFunction
{
public:
    LibraryFunction(const char *id, const char *type, const char *attrs,
                    MetaType metaType)
    {
        _id = std::string(id);
        _type = std::string(type);
        _attrs = std::string(attrs);
        _metaType = metaType;
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

    /**
     * @brief      Get function meta type
     *
     * @return     The function's meta type.
     */
    MetaType getMetaType() const
    {
        return _metaType;
    }
private:
    std::string _id;
    std::string _type;
    std::string _attrs;
    MetaType _metaType;
};
