#pragma once

namespace Util
{
    class ITask
    {
    public:
        virtual void onStart() = 0;
        virtual void onStop() = 0;

        virtual void onService() = 0;
    };
}


