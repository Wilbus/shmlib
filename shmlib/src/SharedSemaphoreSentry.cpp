

#include "SharedSemaphoreSentry.h"

#include <time.h>

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_SENTRY_VALUE 1

namespace shm {

/*inital value is 1 for sentry use case*/
SharedSemaphoreSentry::SharedSemaphoreSentry(std::string semaphoreName)
    : semaphoreName(semaphoreName)
    , execreate(false)
{
    semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_RDWR, SEM_PERMS, INITIAL_SENTRY_VALUE);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }

    wait();
}
SharedSemaphoreSentry::SharedSemaphoreSentry(std::string semaphoreName, bool execreate)
    : semaphoreName(semaphoreName)
    , execreate(execreate)
{
    semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_RDWR, SEM_PERMS, INITIAL_SENTRY_VALUE);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }

    wait();
}

SharedSemaphoreSentry::~SharedSemaphoreSentry()
{
    post();

    if (semaphore == nullptr)
    {
        // do nothing since it was never created
    }
    else if (sem_close(semaphore) < 0)
    {
        perror("sem_close(3) failed");
        /* We ignore possible sem_unlink(3) errors here */
    }
    else
    {
        if (execreate)
            sem_unlink(semaphoreName.c_str());
    }
}

void SharedSemaphoreSentry::wait()
{
    if (sem_wait(semaphore) < 0)
    {
        perror("sem_wait(3) failed");
    }
    // std::cout << "semvalue " << getValue() << "\n";
}

bool SharedSemaphoreSentry::trywait()
{
    if (sem_trywait(semaphore) < 0)
    {
        return false;
    }
    return true;
}

bool SharedSemaphoreSentry::timedwait(unsigned seconds)
{
    struct timespec* ts = new timespec;
    if (clock_gettime(CLOCK_REALTIME, ts) == 1)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    ts->tv_sec += seconds;
    if (sem_timedwait(semaphore, ts) < 0)
    {
        // perror("sem_wait(3) failed");
        return false;
    }
    return true;
    // std::cout << "semvalue " << getValue() << "\n";
}

void SharedSemaphoreSentry::post()
{
    if (sem_post(semaphore) < 0)
    {
        perror("sem_post(3) error on child");
    }
    // std::cout << "semvalue " << getValue() << "\n";
}

void SharedSemaphoreSentry::asyncwait()
{
}

void SharedSemaphoreSentry::unlink()
{
    sem_unlink(semaphoreName.c_str());
}

sem_t* SharedSemaphoreSentry::getSemaphore() const
{
    return semaphore;
}

std::string SharedSemaphoreSentry::getSemaphoreName() const
{
    return semaphoreName;
}

bool SharedSemaphoreSentry::getExeCreate() const
{
    return execreate;
}

int SharedSemaphoreSentry::getValue() const
{
    int* val = new int;
    if (sem_getvalue(semaphore, val) != 0)
    {
        perror("sem_getvalue()");
    }
    return *val;
}
} // namespace shm
