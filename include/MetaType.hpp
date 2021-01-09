#pragma once

/** Enumeration used to carry meta information about the function */
enum class MetaType
{
    Builtin,            /**< Regular builtin */
    StandardLibrary,    /**< Standard library function */
    LanguageBuiltin,    /**< Language-specific builtin */
};