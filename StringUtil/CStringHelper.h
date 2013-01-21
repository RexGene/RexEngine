#pragma once

#include <stdio.h>
#include <string>
#include <vector>

namespace Util
{
    class CStringHelper
    {
    public:
        static bool split(const std::string& inStr, const std::string& delimiter,
                std::vector<std::string>& outString);

        static unsigned int toUInt32(const std::string& inStr);

        static std::string toString(unsigned int value);
    };
}
