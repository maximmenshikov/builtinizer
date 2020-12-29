#pragma once

/** Token position in real type data */
enum class TokenPosition
{
    Before, /**< Position before middle token,
                 e.g. <token> <middle> <after> */
    Middle, /**< Position in the middle of type,
                 e.g. <before> <middle> <after> */
    After, /**< Position after middle token,
                e.g. <before> <middle> <token> */
};
