#pragma once

#include "SharedSemaphore.h"

#include <time.h>

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 0

namespace shm {

SharedSemaphore::SharedSemaphore()
{
}

SharedSemaphore::SharedSemaphore(std::string semaphoreName)
    : semaphoreName(semaphoreName)
{
    semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_RDWR, SEM_PERMS, INITIAL_VALUE);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
}
SharedSemaphore::SharedSemaphore(std::string semaphoreName, bool execreate)
    : semaphoreName(semaphoreName)
    , execreate(execreate)
{
    // if (execreate)
    // semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE);
    // else
    semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_RDWR, SEM_PERMS, INITIAL_VALUE);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
}

SharedSemaphore::SharedSemaphore(const SharedSemaphore& other) // other should get deleted automatically
{
    this->execreate = other.getExeCreate();
    this->semaphoreName = other.getSemaphoreName();
    throw std::runtime_error("test");
    // we expect semaphore file to already be opened in copying
    this->semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_RDWR, SEM_PERMS, INITIAL_VALUE);

    if (this->semaphore == SEM_FAILED)
    {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
}

SharedSemaphore& SharedSemaphore::operator=(const SharedSemaphore& other)
{
    if (this == &other)
    {
        return *this;
    }
    this->execreate = other.getExeCreate();
    this->semaphoreName = other.getSemaphoreName();
    // we expect semaphore file to already be opened in copying
    this->semaphore = sem_open(semaphoreName.c_str(), O_CREAT | O_RDWR, SEM_PERMS, INITIAL_VALUE);

    if (this->semaphore == SEM_FAILED)
    {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }

    return *this;
}

SharedSemaphore::~SharedSemaphore()
{
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

void SharedSemaphore::wait()
{
    if (sem_wait(semaphore) < 0)
    {
        perror("sem_wait(3) failed");
    }
    // std::cout << "semvalue " << getValue() << "\n";
}

bool SharedSemaphore::trywait()
{
    if (sem_trywait(semaphore) < 0)
    {
        return false;
    }
    return true;
}

bool SharedSemaphore::timedwait(unsigned seconds)
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

void SharedSemaphore::post()
{
    if (sem_post(semaphore) < 0)
    {
        perror("sem_post(3) error on child");
    }
    // std::cout << "semvalue " << getValue() << "\n";
}

void SharedSemaphore::asyncwait()
{
}

void SharedSemaphore::unlink()
{
    sem_unlink(semaphoreName.c_str());
}

sem_t* SharedSemaphore::getSemaphore() const
{
    return semaphore;
}

std::string SharedSemaphore::getSemaphoreName() const
{
    return semaphoreName;
}

bool SharedSemaphore::getExeCreate() const
{
    return execreate;
}

int SharedSemaphore::getValue() const
{
    int* val = new int;
    if (sem_getvalue(semaphore, val) != 0)
    {
        perror("sem_getvalue()");
    }
    return *val;
}
} // namespace shm
