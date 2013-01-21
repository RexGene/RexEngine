#pragma once

#include "Util/ITask.h"
#include "CBuffer.h"

#include <string>
#include <map>

namespace Net
{
    class CAcceptor : public Util::ITask
    {
    private:
        typedef std::map<int, Util::CBuffer> IntCBufferMap;
    private:
        std::string _ip;
        unsigned int _port;
        unsigned int _maxListenCount;
        unsigned int _maxFdCount;
        unsigned int _timeout;
        bool _isRunning;
        IntCBufferMap _bufferMap;

    private:
        bool setNonBlocking(int fd);

    public:
        CAcceptor();

        bool init(const std::string& addr);

        virtual void onStart();
        virtual void onStop();
        virtual void onService();
    };
}
