#pragma once

/**
 * Token class with respect to type definition information encoded in Clang's
 * Builtins.def.
 * It is different from TokenPosition in sense that Clang's post modifier might
 * be actually put before the main token.
 */
enum class TokenClass
{
    Unknown,        /**< Unknown token */
    PreModifier,    /**< Type's pre-modifier */
    Type,           /**< Type token */
    PostModifier,   /**< Type's post-modifier */
};
