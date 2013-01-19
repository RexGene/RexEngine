#include "CLog.h"
#include "CDateTime.h"
#include "CAutoLock.h"

#include <sys/stat.h>

using namespace Util;

CLog::CLog()
{

}

void CLog::init(const std::string& dirName)
{
    mkdir(dirName.c_str(), 0777);

    std::string fileName = dirName + "/" + CDateTime().asString("YYYY-MM-DD") + ".log";

    if (NULL == freopen(fileName.c_str(), "a", stdout))
    {
        fprintf(stderr, "error redirecting stdout\n");
        fclose(stderr);
    }
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

    std::cout << stream.str();
}
