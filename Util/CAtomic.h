#pragma once

namespace Util
{
    struct atomic_t
    {
        volatile int counter;

        atomic_t()
        {
            counter = 0;
        }
    };

    inline void atomic_set(atomic_t* v, int i)
    {
        v->counter = i;
    }

    inline void atomic_inc(atomic_t *v)
    {
        __asm__ __volatile__(
                "lock ; incl %0"
                :"=m" (v->counter)
                :"m" (v->counter));
    }

    inline int atomic_dec_and_test(atomic_t *v)
    {
        unsigned char c;
        __asm__ __volatile__(
                "lock ; decl %0; sete %1"
                :"=m" (v->counter), "=qm" (c)
                :"m" (v->counter) : "memory");

        return c != 0;
    } 

}
