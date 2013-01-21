#include "CAcceptor.h"

#include "Util/CLog.h"
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
    CAcceptor::CAcceptor() :
        _maxListenCount(20), _maxFdCount(1024), _isRunning(false), _timeout(500)
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
        int opts = fcntl(fd,F_GETFL);
        if (opts < 0)
        {
            LOG_ERROR("get opts error");
            return false;
        }

        opts |= O_NONBLOCK;

        if (fcntl(fd, F_SETFL, opts) < 0)
        {
            LOG_ERROR("set opts error");
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
        struct epoll_event ev;
        struct epoll_event events[20];

        int epfd = epoll_create(_maxFdCount);
        int listenfd = socket(AF_INET, SOCK_STREAM, 0);

        setNonBlocking(listenfd);

        ev.data.fd=listenfd;
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev); 

        struct sockaddr_in serverAddr;

        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;                                 
        inet_aton(_ip.c_str(), &(serverAddr.sin_addr));
        serverAddr.sin_port = htons(_port);

        bind(listenfd, (sockaddr *)&serverAddr, sizeof(serverAddr));      

        listen(listenfd, _maxListenCount);

        while (_isRunning)
        {
            int nfds = epoll_wait(epfd, events, 
                    sizeof(events) / sizeof(struct epoll_event), _timeout);

            for (int i = 0; i < nfds; ++i)
            {
                struct epoll_event& event = events[i];
                if (event.data.fd == listenfd)
                {
                    socklen_t socklen;
                    struct sockaddr_in clientAddr;

                    int connfd = accept(listenfd, (sockaddr*)&clientAddr, &socklen);
                    if (connfd < 0)
                    {
                        LOG_ERROR("accept error");
                        continue;
                    }

                    setNonBlocking(connfd);

                    ev.data.fd=connfd;
                    ev.events=EPOLLIN | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);

                    std::pair<IntCBufferMap::iterator, bool> result = 
                        _bufferMap.insert(std::make_pair(connfd, Util::CBuffer()));

                    if (!result.second)
                    {
                        LOG_ERROR("socket is already exist!" <<
                                " socket:" << connfd);

                        result.first->second.clear();
                    }
                }
                else if (event.events & EPOLLIN)
                {
                    _currentSocket = event.data.fd;
                    if (-1 == _currentSocket)
                    {
                        continue;
                    }

                    IntCBufferMap::iterator iter = _bufferMap.find(_currentSocket);
                    if (iter == _bufferMap.end())
                    {
                        LOG_ERROR("can not found socket:" << _currentSocket);
                    }

                    Util::CBuffer& dataBuffer = iter->second;

                    unsigned int remainSpace = dataBuffer.getRemainSpace();
                    if (0 == remainSpace)
                    {
                        dataBuffer.appendSpace();
                        remainSpace = dataBuffer.getRemainSpace();
                    }
                    int n = read(_currentSocket, dataBuffer.getBuffer,
                            remainSpace);

                    if (n <= 0)
                    {
                        dataBuffer.clear();
                        close(_currentSocket);
                        event.data.fd = -1;

                        LOG_ERROR("read data error");
                    }

                    dataBuffer.appendSize(n);

                    //[TODO]
                }
            }
        }
    }
}


