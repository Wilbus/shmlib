#pragma once

// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

namespace shm {
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

template <class T>
class SharedMemory
{
public:
    SharedMemory()
    {
    }

    SharedMemory(std::string nm, size_t sz, bool newMem)
    {
        name = nm;

        if (newMem)
        {
            std::cout << "test0\n";
            shmid = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
            ftruncate(shmid, sz);
            shm = static_cast<T*>(mmap(0, sz, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0));
            size = getSize();
        }
        else
        {
            std::cout << "test1\n";
            shmid = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
            shm = static_cast<T*>(mmap(0, sz, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0));
            size = getSize();

            /*std::cout << "eread test\n";
            for(unsigned i = 0; i < size; i++)
            {
                std::cout << shm[i]; //it exists
            }*/
        }

        if (shmid < 0)
        {
            perror("shm_open");
            throw std::runtime_error("shm_open error");
        }
    }

    SharedMemory(std::string nm)
    {
        name = nm;

        shmid = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        size = getSize();
        shm = static_cast<T*>(mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0));

        if (shmid < 0)
        {
            perror("shm_open");
            throw std::runtime_error("shm_open error");
        }
    }

    SharedMemory(const SharedMemory& other)
    {
        shmid = other.getShmid();
        size = other.getSize();
        name = other.getName();
        // shm = static_cast<T*>(shmat(shmid, NULL, 0));
        shm = static_cast<T*>(mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0));
        if (static_cast<void*>(shm) == nullptr)
        {
            perror("shmat");
            throw std::runtime_error("shmat error");
        }
    }

    SharedMemory& operator=(const SharedMemory& other)
    {
        if (this == &other)
        {
            return *this;
        }

        shmid = other.getShmid();
        size = other.getSize();
        name = other.getName();
        // shm = static_cast<T*>(shmat(shmid, NULL, 0));
        shm = static_cast<T*>(mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0));
        if (static_cast<void*>(shm) == nullptr)
        {
            perror("shmat");
            throw std::runtime_error("shmat error");
        }

        return *this;
    }

    ~SharedMemory()
    {
        /*if (shmdt(static_cast<void*>(shm)) < 0)
        {
            std::printf("%p\n", (void*)shm);
            perror("shmdt");
        }*/
        /*if (newMem)
        {
            shmctl(shmid, IPC_RMID, NULL);
        }*/
        /*if(newMem) //if we don't call, memory is invalidated only when process terminates
        {
            munmap(static_cast<void*>(shm), size);
            shm_unlink(name.c_str());
        }*/
        // munmap(static_cast<void*>(shm), size);
        // shm_unlink(name.c_str());
    }

    /*workaround: we force memory to stay active until explicitly told to release*/
    void markForRelease()
    {
        /*if (shmctl(shmid, IPC_RMID, NULL) < 0)
        {
            perror("shmctl");
        }*/
        munmap(static_cast<void*>(shm), size);
        shm_unlink(name.c_str());
    }

    T* get() const
    {
        return shm;
    }

    int getShmid() const
    {
        return shmid;
    }

    uint64_t getSize() const
    {
        // return size;
        struct stat buf;
        if (fstat(shmid, &buf) < 0)
        {
            perror("fstat");
            throw std::runtime_error("fstat error");
        }
        return buf.st_size;
    }

    std::string getName() const
    {
        return name;
    }

private:
    int shmid;
    T* shm;
    std::string name;
    size_t size;
    bool newMem;
};
} // namespace shm
