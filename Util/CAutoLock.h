#pragma once

#include "ILock.h"

namespace Util
{
    class CAutoLock
    {
        private:
            ILock* _pLock;

        public:
            CAutoLock(ILock* pLock);
            ~CAutoLock();
    };
}
