#include "CTaskManager.h"
#include "CAutoLock.h"

#include <stdio.h>

using namespace Util;

CTaskManager::CTaskManager()
{   
}

const ITask* CTaskManager::find(pthread_t tid)
{
    std::map<pthread_t, ITask*>::iterator iter = _tidMap.find(tid);
    if (iter == _tidMap.end())
    {
        return NULL;
    }

    return iter->second;
}

bool CTaskManager::start(ITask* pTask)
{
    pthread_t tid = 0;
    CAutoLock autoLock(&_lock);

    if (_freeList.empty())
    {
        bool result = true;

        pTask->onStart();
        pthread_create(&tid, NULL, (TaskFunction)handleTask, (void*)pTask);

        result &= _tidMap.insert(std::make_pair(tid, pTask)).second;
        result &= _taskMap.insert(std::make_pair(pTask, tid)).second;

        return result;
    }
    else
    {
        tid = *_freeList.begin();
        _freeList.pop_front();

        std::map<pthread_t, ITask*>::iterator iter = _tidMap.find(tid);
        if (iter == _tidMap.end())
        {
            return false;
        }

        iter->second = pTask;

        pTask->onStart();
        pthread_continue(tid);

        return true;
    }

}

void CTaskManager::stop(ITask* pTask)
{
    pTask->onStop();
}

void CTaskManager::handleTask(ITask* pTask)
{
    std::map<ITask*, pthread_t>& taskMapRef = theTaskManager._taskMap;
    std::map<pthread_t, ITask*>& tidMapRef = theTaskManager._tidMap;
    std::map<ITask*, pthread_t>::iterator iter;
    std::map<pthread_t, ITask*>::iterator tidIter;

    do
    {
        iter = taskMapRef.find(pTask);
    }while (iter == taskMapRef.end());

    pthread_t tid = pthread_self();

    std::list<pthread_t>& freeListRef = theTaskManager._freeList;
    do
    {
        tidIter = tidMapRef.find(tid);
        if (tidIter == tidMapRef.end())
        {
            return;
        }

        pTask = tidIter->second;

        pTask->onService();
        
        freeListRef.push_back(tid);
        pthread_suspend(tid);
        printf("tid=%d\n", tid);
    }while (true);
}

