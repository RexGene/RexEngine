#include "CAutoLock.h"

using namespace Util;

CAutoLock::CAutoLock(ILock* pLock)
{
    _pLock = pLock;
    _pLock->lock();
}

CAutoLock::~CAutoLock()
{
    _pLock->unlock(); 
}

