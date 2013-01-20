#pragma once

#include "ILock.h"

#ifdef _LINUX
#   include <pthread.h>
#endif

namespace Util
{
    class CLock : public virtual ILock
    {
        public:
            CLock()
            {
#ifdef _WIN32
#if defined(_WIN32_WINNT) && (_WIN32_WINNT > 0x0403)
                InitializeCriticalSectionAndSpinCount(&_lock , 10 );
#else
                InitializeCriticalSection(&_lock);
#endif
#elif defined(_LINUX)
                pthread_mutexattr_t attr;
                pthread_mutexattr_init(&attr);

                pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);

                pthread_mutex_init(&_lock,&attr);
                pthread_mutexattr_destroy(&attr);
#endif
            }
            virtual ~CLock()
            {
#ifdef _WIN32
                DeleteCriticalSection(&_lock);
#elif defined(_LINUX)
                pthread_mutex_destroy(&_lock);
#endif
            }

            inline virtual void lock()
            {
#ifdef _WIN32
                EnterCriticalSection(&_lock);
                //assert( _lock.RecursionCount <= 1 );
#elif defined(_LINUX)
                if( 0 != pthread_mutex_lock(&_lock) )
                {
#ifdef _DEBUG
                    //  if( errno == EDEADLK )
                    //  {
                    //      assert( false );
                    //  }
#endif
                }
#endif
            }

            inline virtual void unlock()
            {
#ifdef _WIN32
                LeaveCriticalSection(&_lock);
#elif defined(_LINUX)
                pthread_mutex_unlock(&_lock);
#endif
            }
        private:
            CLock( const CLock& )
            {
            }
#ifdef _WIN32
            CRITICAL_SECTION _lock;
#elif defined(_LINUX)
            pthread_mutex_t _lock;
#endif
    };
}
