#include "CLog.h"
#include "CDateTime.h"
#include "CAutoLock.h"

#include <sys/stat.h>

using namespace Util;

CLog::CLog()
{

}

void CLog::bind(const std::string& dirName, FILE* pFileHandle)
{
    mkdir(dirName.c_str(), 0754);

    std::string fileName = dirName + "/" + CDateTime().asString("YYYY-MM-DD") + ".log";

    if (NULL == freopen(fileName.c_str(), "a", pFileHandle))
    {
        fprintf(pFileHandle, "error redirecting\n");
    }
}

void CLog::init(const std::string& dirName)
{
    bind(dirName, stdout);
    bind(dirName + "_Error", stderr);
}

void CLog::log(ELogType logType, const std::string& content)
{
    CAutoLock lock(getLock());

    std::ostringstream stream;

    stream << "<" << CDateTime().asString() << "> ";

    switch ((int)logType)
    {
        case ELogType_Debug:
            stream << "[Debug]";
            break;

        case ELogType_Info:
            stream << "[Info]";
            break;

        case ELogType_Error:
            stream << "[Error]";
            break;
    }

    stream << content << std::endl;

    if (ELogType_Error == logType)
    {
        std::cerr << stream.str();
    }
    else
    {
        std::cout << stream.str();
    }

}
