#pragma once

#include "Util/ITask.h"

#include <string>

namespace Net
{
    class CAcceptor : public Util::ITask
    {
    private:
        std::string _ip;
        unsigned int _port;
        unsigned int _maxListenCount;
        unsigned int _maxFdCount;
        unsigned int _timeout;
        bool _isRunning;

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
