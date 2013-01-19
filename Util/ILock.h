#pragma once

namespace Util
{
    class ILock
    {
        public:
            virtual void lock() = 0;
            virtual void unlock() = 0;
    };
}
