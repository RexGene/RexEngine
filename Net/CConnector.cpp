#include "CAcceptor.h"

#include "Util/CStringHelper.h"
#include "Util/CLog.h"

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
        struct sockaddr_in addr;
        _connect_fd = socket(AF_INET, SOCK_STREAM, 0);

        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(_ip);
        addr.sin_port = htons(_port);

CONNECT:
        do 
        {
            int ret = connect(_connect_fd, 
                    (struct sockaddr*)&addr, sizeof(addr));

            if (SOCKET_ERROR == ret)
            {
                LOG_ERROR("connect error!" <<
                        " ip:" << _ip <<
                        " port:" << _port);

                sleep(100);
            }
        }while (SOCKET_ERROR == ret && _isRunning);

        while (_isRunning)
        {
            unsigned int remainSpace = _buffer.getRemainSpace();
            if (0 == remainSpace)
            {
                _buffer.appendSpace();
                remainSpace = _buffer.getRemainSpace();
            }

            int len = read(_connect_fd, _buffer.getBuffer(), remainSpace);
            if (len > 0)
            {
                _buffer.appendSize(len);

                LOG_DEBUG("data in");
                //[TODO]
            }
            else
            {
                LOG_ERROR("read error!" <<
                        " socket:" << _connect_fd <<
                        " len:" << len);

                closesocket(_connect_fd);
                _buffer.clear();

                goto CONNECT;
            }
        }
    }

    
    bool CConnector::send(void* pData, unsigned int size)
    {
        
    }
}

