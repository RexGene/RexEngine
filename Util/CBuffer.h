#pragma once

namespace Util
{
    class CBuffer
    {
    private:
        uint8_t* _pBuffer;
        uint8_t* _pBegin;
        uint8_t* _pEnd;
        unsigned int _size;

    public:
        CBuffer(unsigned int size = 1024);
        ~CBuffer();

        void reset();
        void clear();

        uint8_t readByte();
        uint16_t readUInt16();
        uint32_t readUInt32();
        uint64_t readUInt64();

        int8_t readSByte();
        int16_t readInt16();
        int32_t readInt32();
        int64_t readInt64();

        void append(void* pData, unsigned int size);
        void appendSize(unsigned int size);
        void appendSpace(unsigned int space = 0);
        unsigned int getLength();
        unsigned int getSize();
        unsigned int getRemainSpace();
    };
}
