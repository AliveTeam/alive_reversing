#include "asset_common.hpp"

std::string StringToLowerCase(std::string text)
{
    // make copy of string
    std::string lowerCase = text;

    // convert to lower case but cast to char
    std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), [](int c) -> char { return static_cast<char>(::tolower(c)); });

    return lowerCase;
}