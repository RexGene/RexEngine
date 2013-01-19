#include "Util/CTaskManager.h"
#include "Util/ITask.h"

#include <stdio.h>

class A : public Util::ITask
{
public:
    A()
    {

    }

    virtual void onStart()
    {
        
    }

    virtual void onStop()
    {
        
    }

    virtual void onService()
    {
        static int i = 0;

        printf("i=%d\n", i++);
    }

};

int main(int argc, char* args[])
{
    return 0;
}
