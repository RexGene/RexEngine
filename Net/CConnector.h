#pragma once

#include "CBuffer.h"
#include "Util/ITask.h"

#include <string>

namespace Net
{
    class CConnector : public Util::ITask
    {
    private:
        Util::CBuffer _buffer;

    public:
        CConnector();

        bool init(const std:string& addr);

        virtual void onStart();
        virtual void onStop();
        virtual void onService();

        bool send(void* pData, unsigned int size);
    };
}
