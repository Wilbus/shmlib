#include "SharedRingBuffer.h"

#include <stdio.h>
#include <unistd.h>

using namespace shm;

unsigned buffsize = 100000;
unsigned popcount1 = 0;
unsigned popcount2 = 0;

void syscall0(std::string arg)
{
    // Launch child process
    std::string s(arg);
    s += " 2";
    std::cout << "start t1\n";
    if (0 != std::system(s.c_str()))
    {
    }
}

void syscall1(std::string arg)
{
    // Launch child process
    std::string s(arg);
    std::string args = " 2 3";
    s += args;
    std::cout << "start t2\n";
    if (0 != std::system(s.c_str()))
    {
    }
}

void syscall2(std::string arg)
{
    // Launch child process
    std::string s(arg);
    std::string args = " 2 3 4";
    s += args;
    std::cout << "start t3\n";
    if (0 != std::system(s.c_str()))
    {
    }
}

void pushfunc()
{
    auto pid = getpid();
    std::cout << pid << " pushfunc() start\n";
    SharedRingBuffer ringbuff = SharedRingBuffer(111, buffsize, false);
    for (unsigned i = 0; i < buffsize; i++)
    {
        std::printf("%d curr getHeadIdx: %lu\n", pid, ringbuff.getHeadIdx());
        std::printf("%d curr getIdx: %lu\n", pid, ringbuff.getIdx());
        if (!ringbuff.push(64))
        {
            throw std::runtime_error("full");
        }
    }
    std::cout << pid << " pushfunc() end\n";
}

void popfunc()
{
    auto pid = getpid();
    std::cout << pid << " popfunc() start\n";
    SharedRingBuffer ringbuff = SharedRingBuffer(111, buffsize, false);
    for (unsigned i = 0; i < buffsize; i++)
    {
        std::printf("%d child curr getHeadIdx: %lu\n", pid, ringbuff.getHeadIdx());
        std::printf("%d child curr getIdx: %lu\n", pid, ringbuff.getIdx());
        if (ringbuff.isEmpty())
        {
            std::cout << "error empty\n";
            continue;
        }
        std::cout << ringbuff.front() << "\n";
        ringbuff.pop();
        popcount1++;
    }

    // ringbuff.releaseBuffer();
    std::cout << pid << " popfunc() end popcount1 = " << popcount1 << "\n";
}

void popfunc2()
{
    auto pid = getpid();
    std::cout << pid << " popfunc()2 start\n";
    SharedRingBuffer ringbuff = SharedRingBuffer(111, buffsize, false);
    for (unsigned i = 0; i < buffsize; i++)
    {
        std::printf("%d child2 curr getHeadIdx: %lu\n", pid, ringbuff.getHeadIdx());
        std::printf("%d child2 curr getIdx: %lu\n", pid, ringbuff.getIdx());
        if (ringbuff.isEmpty())
        {
            std::cout << "error empty\n";
            continue;
        }
        std::cout << ringbuff.front() << "\n";
        ringbuff.pop();
        popcount2++;
    }

    std::cout << pid << " popfunc()2 end, popcount2 = " << popcount2 << "\n";
}

int main(int argc, char* argv[])
{
    SharedRingBuffer ringbuff;

/*sequential puush pop*/
#if 0
    if (argc == 1)
    {
        ringbuff = SharedRingBuffer(111, buffsize, true);
        //SharedRingBuffer ringbuff = SharedRingBuffer(111, buffsize, true);

        for (unsigned i = 0; i < buffsize; i++)
        {
            //std::printf("curr getHeadIdx: %lu\n", ringbuff.getHeadIdx());
            //std::printf("curr getIdx: %lu\n", ringbuff.getIdx());
            if (!ringbuff.push(64))
            {
                throw std::runtime_error("full");
            }
        }

        // Launch child process
        std::string s(argv[0]);
        s += " child ";
        if (0 != std::system(s.c_str()))
            return 1;
    }
    else
    {
        ringbuff = SharedRingBuffer(111, buffsize, false);

        std::printf("child size: %lu\n", ringbuff.size());
        for (unsigned i = 0; i < buffsize; i++)
        {
           // std::printf("child curr getHeadIdx: %lu\n", ringbuff.getHeadIdx());
            //std::printf("child curr getIdx: %lu\n", ringbuff.getIdx());
            if (ringbuff.isEmpty())
            {
                throw std::runtime_error("empty");
            }
            std::cout << ringbuff.front() << "\n";
            ringbuff.pop();
        }

        ringbuff.releaseBuffer();
    }
    return 0;
#endif

    /*2 process push pop*/
    std::string arg = argv[0];
    if (argc == 1)
    {
        // SharedSemaphore ss(sem0, true);
        // ss.post();
        // SharedSemaphoreSentry ss1(sem1, true);

        /*// Launch child process
        std::string s(argv[0]);
        s += " child ";
        if (0 != std::system(s.c_str()))
            return 1;*/
        SharedRingBuffer ringbuff = SharedRingBuffer(111, buffsize, true);

        std::thread t1process(syscall0, arg);
        std::thread t2process(syscall1, arg);
        std::thread t3process(syscall2, arg);

        t1process.join();
        t2process.join();
        t3process.join();

        ringbuff.releaseBuffer();
    }
    else if (argc == 2)
    {
        pushfunc();
    }
    else if (argc == 3)
    {
        popfunc();
        // popfunc();
    }
    else if (argc == 4)
    {
        popfunc2();
        // popfunc();
    }
}