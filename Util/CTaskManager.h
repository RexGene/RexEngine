#pragma once

#include "ITask.h"
#include "CLock.h"
#include "CSingleton.h"

#include <map>
#include <list>
#include <pthread.h>

namespace Util
{
    class CTaskManager
    {
    private:
        typedef void* (*TaskFunction)(void *);

    private:
        static void handleTask(ITask* pTask);

    public:
        CTaskManager();

        bool start(ITask* pTask);
        void stop(ITask* pTask);
    };
}

#define theTaskManager Util::CSingleton::getInstance()
