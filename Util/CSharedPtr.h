#pragma once

#include "CRefCount.h"

#define NULL (0)

namespace Util
{
    template <typename T> class CSharedPtr
    {
        private:
            typedef CSharedPtr<T> SelfType;
            typedef CRefCount RefCount;

        private:
            T* _objectPtr;

        public:
            RefCount* _refCountPtr;

        private:
            void reset()
            {
                if (_objectPtr)
                {
                    if (_refCountPtr->decRef())
                    {            
                        delete _objectPtr;
                    }

                    _objectPtr = NULL;
                    _refCountPtr = NULL;    
                }
            }

        public:
            CSharedPtr<T>(T* objectPtr = NULL) : 
                _objectPtr(objectPtr),
                _refCountPtr(NULL)
        {
            if (NULL != objectPtr)
            {
                _refCountPtr = new RefCount();
                _refCountPtr->incRef();
            }
        }

            CSharedPtr<T>(const SelfType& objectPtr) :
                _objectPtr(objectPtr.get()),
                _refCountPtr(objectPtr.getRefCountPtr())
        {
            if (NULL != _objectPtr)
            {
                _refCountPtr->incRef();
            }
        }

            template <typename TClass>
                CSharedPtr<T>(const TClass& objectPtr) :
                    _objectPtr(static_cast<T*>(objectPtr.get())),
                    _refCountPtr(objectPtr.getRefCountPtr())
        {
            if (NULL != _objectPtr)
            {
                _refCountPtr->incRef();
            }
        }

            ~CSharedPtr()
            {
                reset();
            }

            RefCount* getRefCountPtr() const
            {
                return _refCountPtr;
            }

            T* get() const
            {
                return _objectPtr;
            }

            T& operator*() const
            {
                return *_objectPtr;
            }

            T* operator->() const
            {   
                return _objectPtr;
            }

            template <typename TClass>
                SelfType& operator=(const TClass& objectPtr)
                {
                    if (NULL != objectPtr.get())
                    {
                        _objectPtr = static_cast<T>(objectPtr.get());
                        _refCountPtr = objectPtr.getRefCountPtr();
                        _refCountPtr->incRef();
                    }
                    else
                    {
                        reset();
                    }

                    return *this;
                }

            SelfType& operator=(const SelfType& objectPtr)
            {
                if (NULL != objectPtr.get())
                {
                    _objectPtr = objectPtr.get();
                    _refCountPtr = objectPtr.getRefCountPtr();
                    _refCountPtr->incRef();
                }
                else
                {
                    reset();
                }

                return *this;
            }

            SelfType& operator=(const T* objectPtr)
            {
                if (NULL != objectPtr)
                {
                    _objectPtr = objectPtr;
                    _refCountPtr = new RefCount();
                    _refCountPtr->incRef();
                }
                else
                {
                    reset();
                }

                return *this;
            }

            inline bool operator==(const SelfType& objectPtr) const
            {
                return _objectPtr == objectPtr.get();
            }

            inline bool operator==(const T* objectPtr) const
            {
                return _objectPtr == objectPtr;
            }

            inline bool operator<(const SelfType& objectPtr) const
            {
                return _objectPtr < objectPtr.get();
            }

            inline bool operator<(const T* objectPtr) const
            {
                return _objectPtr < objectPtr;
            }

            inline bool operator>(const SelfType& objectPtr) const
            {
                return _objectPtr > objectPtr.get();
            }

            inline bool operator>(const T* objectPtr) const
            {
                return _objectPtr > objectPtr;
            }

            inline bool operator>=(const SelfType& objectPtr) const
            {
                return _objectPtr >= objectPtr.get();
            }

            inline bool operator>=(const T* objectPtr) const
            {
                return _objectPtr >= objectPtr;
            }

            inline bool operator<=(const SelfType& objectPtr) const
            {
                return _objectPtr <= objectPtr.get();
            }

            inline bool operator<=(const T* objectPtr) const
            {
                return _objectPtr >= objectPtr;
            }
    };
}

