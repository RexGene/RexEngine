#pragma once

#include "CLock.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

namespace Util
{
    class CLog
    {
        public:
            enum ELogType
            {
                ELogType_Debug,
                ELogType_Info,
                ELogType_Error,
            };

        private:
            inline static ILock* getLock()
            {
                static CLock lock;
                return &lock;
            }

            void bind(const std::string& dirName, FILE* pFileHandle);

        public:
            CLog();

            void init(const std::string& dirName);
            void log(ELogType logType, const std::string& content);
            
            inline static CLog& getInstance()
            {
                static CLog instance;
                return instance;
            }
    };
}

#define theLog CLog::getInstance()

#ifdef _DEBUG
#   define LOG_DEBUG(content) \
    { \
        std::ostringstream stream; \
        stream << content; \
        Util::CLog::getInstance().log(Util::CLog::ELogType_Debug, stream.str()); \
    }
#else
#   define LOG_DEBUG(content)
#endif

#define LOG_INFO(content) \
{ \
    std::ostringstream stream; \
    stream << content; \
    Util::CLog::getInstance().log(Util::CLog::ELogType_Info, stream.str()); \
}

#define LOG_ERROR(content) \
{ \
    std::ostringstream stream; \
    stream << content; \
    Util::CLog::getInstance().log(Util::CLog::ELogType_Error, stream.str()); \
}

