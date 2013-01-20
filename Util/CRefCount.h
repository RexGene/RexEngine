#pragma once

#include "CAtomic.h"

namespace Util
{
    class CRefCount
    {
        private:
            atomic_t _count;

        public:
            CRefCount() 
            {
            };

            inline void incRef()
            {
                atomic_inc(&_count);
            }

            inline bool decRef()
            {
                return (bool)atomic_dec_and_test(&_count);
            }
    };
}
