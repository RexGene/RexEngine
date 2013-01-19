#pragma once

#include "ITask.h"
#include "CLock.h"

#include <map>
#include <list>
#include <pthread.h>

namespace Util
{
    class CTaskManager
    {
    private:
        typedef void* (*TaskFunction)(void*);

    private:
        CLock _lock;
        std::map<ITask*, pthread_t> _taskMap;
        std::map<pthread_t, ITask*> _tidMap;
        std::list<pthread_t> _freeList;

    private:
        const ITask* find(pthread_t);

        static void handleTask(ITask* pTask);

    public:
        CTaskManager();

        bool start(ITask* pTask);
        void stop(ITask* pTask);

        inline static CTaskManager& getInstance()
        {
            static CTaskManager instance;
            return instance;
        }
    };
}

#define theTaskManager Util::CTaskManager::getInstance()
