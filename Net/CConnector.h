#pragma once

#include "CBuffer.h"
#include "Util/ITask.h"

#include <string>

namespace Net
{
    class CConnector : public Util::ITask
    {
    private:
        CBuffer _buffer;

    public:
        CConnector();

        bool init(const std:string& addr);

        virtual void onStart();
        virtual void onStop();
        virtual void onService();
    };
}
