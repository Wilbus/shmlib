#pragma once

#include "ISharedSemaphore.h"

#include <time.h>

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 0

namespace shm {
/*works only as a binary semaphore at the moment*/
class SharedSemaphoreSentry : public ISharedSemaphore
{
public:
    SharedSemaphoreSentry();
    SharedSemaphoreSentry(std::string semaphoreName);
    SharedSemaphoreSentry(std::string semaphoreName, bool execreate);

    SharedSemaphoreSentry(const SharedSemaphoreSentry& other);

    SharedSemaphoreSentry& operator=(const SharedSemaphoreSentry& other);

    ~SharedSemaphoreSentry();
    virtual void wait() override;

    virtual bool trywait() override;

    virtual bool timedwait(unsigned seconds) override;

    virtual void post() override;

    virtual void asyncwait() override;

    virtual void unlink() override;

    virtual sem_t* getSemaphore() const override;

    virtual std::string getSemaphoreName() const override;

    virtual bool getExeCreate() const override;

    virtual int getValue() const override;

private:
    std::string semaphoreName;
    bool execreate;
    sem_t* semaphore;
    int value{INITIAL_VALUE};
};
} // namespace shm