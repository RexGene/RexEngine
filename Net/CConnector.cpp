#include "CAcceptor.h"

#include "Util/CStringHelper.h"

#include <strings.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace Util;

namespace Net
{
    CConnector::CConnector() : _isRunning(false), _timeout(500)
    {

    }

    bool CAcceptor::init(const std::string& addr)
    {
        std::vector<std::string> outStringArray;
        CStringHelper::split(addr, ":", outStringArray);

        if (2 != outStringArray.size())
        {
            return false;
        }

        _ip = outStringArray[0];
        _port = CStringHelper::toUInt32(outStringArray[1]);

        return true;
    }

    bool CAcceptor::setNonBlocking(int fd)
    {
        int opts = fcntl(fd, F_GETFL);
        if (opts < 0)
        {
            printf("get opts error\n");
            return false;
        }

        opts |= O_NONBLOCK;

        if (fcntl(fd, F_SETFL, opts) < 0)
        {
            printf("set opts error\n");
            return false;
        }
    }

    void CAcceptor::onStart()
    {
        _isRunning = true;
    }

    void CAcceptor::onStop()
    {
        _isRunning = false;
    }

    void CAcceptor::onService()
    {
        

    }
}

