#include "Util/CSharedPtr.h"
#include "Util/CLog.h"

#include<unistd.h> 
#include<sys/types.h>

using namespace Util;

int main(int argc, char* args[])
{
    if (0 != fork())
    {
        return 1;
    }

    theLog.init(args[0]);

    LOG_INFO("hi" << 2 << "!");

    return 0;
}
