#pragma once

namespace Util
{
    template <class T>
    class CSingleton
    {
        static T& getInstance()
        {
            static T s_instance;
            return s_instance;
        }
    };
}
