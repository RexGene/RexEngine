#include "CStringHelper.h"

#include <stdlib.h>
#include <string.h>

namespace Util
{
    bool CStringHelper::split(const std::string& inStr, const std::string& delimiter,
                std::vector<std::string>& outStringArray)
    {
        const char* ps_str = inStr.c_str(); 
        const char* ps_split = delimiter.c_str();
        const int len = (int)strlen(ps_split);

        if (!len)
        {
            return false;
        }

        outStringArray.clear();

        const char * pstrend = ps_str + strlen(ps_str);
        const char * pstr = ps_str;
        const char * pthuckstart = ps_str;
        while ((pstr = strstr(pthuckstart , ps_split)) != NULL)
        {
            std::string str(pthuckstart ,pstr - pthuckstart);
            if (str.length())
            {
                outStringArray.push_back(str);
            }
            pthuckstart = pstr + len;
        }

        if( pstrend != pthuckstart )
        {
            outStringArray.push_back(std::string(pthuckstart));
        }

        return true;
    }


    unsigned int CStringHelper::toUInt32(const std::string& inStr)
    {
        unsigned int result = 0;

        if (inStr.empty())
        {
            return result;
        }

        const char* pReader = inStr.c_str();

        while ('\0' != *pReader)
        {
            if (*pReader < '0' || *pReader > '9')
            {
                continue;
            }

            result *= 10;
            result += *pReader++ - '0';
        }

        return result;
    }

    std::string CStringHelper::toString(unsigned int value)
    {
        char buffer[16] = {0};
        char outBuffer[16] = {0};
        char* pReader = &buffer[0];
        char* pWriter = &outBuffer[0];

        do
        {
            *pReader++ = value % 10;
            value /= 10;
        }while (value);

        do
        {
            *pWriter++ = *--pReader;
        }while (pReader >= buffer);

        *pWriter = '\0';

        return outBuffer;
    }
}
