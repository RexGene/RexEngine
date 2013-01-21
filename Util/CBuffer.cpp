#include "CBuffer.h"

#include <memory>

namespace Util
{
    CBuffer::CBuffer(unsigned int size) 
        : _pBuffer(NULL), _pBegin(NULL), _pEnd(NULL)
    {
        _size = size;
        _pBegin = _pEnd = _pBuffer = (uint8_t*)malloc(_size);
    }

    CBuffer::~CBuffer()
    {
        free(_pBuffer);
    }

    void CBuffer::reset()
    {
        _pBegin = _pBuffer;
    }

    void CBuffer::clear()
    {
        _pBegin = _pBuffer;
        _pEnd = _pBuffer;
    }

    uint8_t CBuffer::readByte()
    {
        return *((uint8_t*)pBegin)++;
    }

    uint16_t CBuffer::readUInt16()
    {
        return *((uint16_t*)pBegin)++;
    }

    uint32_t CBuffer::readUInt32()
    {
        return *((uint32_t*)pBegin)++;
    }

    uint64_t CBuffer::readUInt64()
    {
        return *((uint64_t*)pBegin)++;
    }

    int8_t CBuffer::readSByte()
    {
        return *((int8_t*)pBegin)++;
    }

    int16_t CBuffer::readInt16()
    {
        return *((int16_t*)pBegin)++;
    }

    int32_t CBuffer::readInt32()
    {
        return *((int32_t*)pBegin)++;
    }

    int64_t CBuffer::readInt64()
    {
        return *((int64_t*)pBegin)++;
    }

    unsigned int getLength()
    {
        if (pBegin > _pEnd)
        {
            _pBegin = _pEnd;
        }

        return _pEnd - pBegin;
    }

    unsigned int getSize()
    {
        return _pEnd - pBuffer;
    }

    void CBuffer::append(void* pData, unsigned int size)
    {
        while (getSize() + size > _size)
        {
            appendSpace();
        }

        memcpy(_pEnd, pData, size);
        _pEnd += size;
    }

    void* CBuffer::getBuffer()
    {
        return _pEnd;
    }

    unsigned int CBuffer::getRemainSpace()
    {
        return _size - (_pEnd - _pBuffer);
    }

    void CBuffer::appendSize(unsigned int size)
    {
        return _pEnd += size;
    }

    void appendSpace(unsigned int space)
    {
        int beginPos = _pBegin - _pBuffer;
        int endPos = _pEnd - pBuffer;     
      
        if (0 == space)
        {
            _size <<= 1;
        }
        else
        {
            _size += space;
        }
                                          
        pBuffer = realloc(pBuffer, _size);
                                          
        _pBegin = _pBuffer + beginPos;    
        _pEnd = _pBuffer + endPos;        
    }
}
