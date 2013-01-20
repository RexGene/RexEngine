#include "CTaskManager.h"

#include <stdio.h>

using namespace Util;

CTaskManager::CTaskManager()
{   
}

bool CTaskManager::start(ITask* pTask)
{
    pthread_t tid = 0;

    pTask->onStart();
    pthread_create(&tid, NULL, (TaskFunction)handleTask, (void*)pTask);
}

void CTaskManager::stop(ITask* pTask)
{
    pTask->onStop();
}

void CTaskManager::handleTask(ITask* pTask)
{
    pTask->onService();
}

