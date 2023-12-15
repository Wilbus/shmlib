#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

namespace shm {

class ISharedSemaphore
{
public:
    virtual void wait() = 0;
    virtual bool timedwait(unsigned seconds) = 0;
    virtual bool trywait() = 0;
    virtual void post() = 0;
    virtual void asyncwait() = 0;
    virtual void unlink() = 0;
    virtual sem_t* getSemaphore() const = 0;
    virtual std::string getSemaphoreName() const = 0;
    virtual bool getExeCreate() const = 0;
    virtual int getValue() const = 0;
};
} // namespace shm