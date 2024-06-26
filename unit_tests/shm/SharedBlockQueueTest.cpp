#include "SharedBlockQueue.h"

#include <cassert>
#include <random>
#include <stdio.h>
#include <unistd.h>

using namespace shm;

size_t size = 1e8; // max size holding structures of varying blocksize
key_t key = 1234;

size_t blocksize = 150; // max size of one block

unsigned counter1 = 0;

unsigned generateRandomInt()
{
    std::random_device seed;
    std::mt19937 gen{seed()};                    // seed the generator
    std::uniform_int_distribution<> dist{1, 150}; // set min and max
    unsigned guess = dist(gen);                  // generate number
    return guess;
}

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

void syscall3(std::string arg)
{
    // Launch child process
    std::string s(arg);
    std::string args = " 2 3 4 5";
    s += args;
    std::cout << "start t3\n";
    if (0 != std::system(s.c_str()))
    {
    }
}

void pushfunc()
{
    SharedBlockQueue queue = SharedBlockQueue(key, size, false);

    unsigned blockcount = 0;
    for (unsigned i = 0; i < size / blocksize; i++) // push size / vecsize blocks
    {
        auto randomBlockSize = generateRandomInt();
        std::vector<uint8_t> bytes0(randomBlockSize, 0);
        unsigned counter = 0;
        for (unsigned i = 0; i < randomBlockSize; i++)
        {
            if (counter == 0)
                bytes0[i] = 6;
            if (counter == randomBlockSize - 1)
                bytes0[i] = 9;
            counter++;
        }
        if (queue.writeblock(bytes0))
        {
            blockcount++;
            std::vector<uint8_t> bytes;
            // readfront()
        }
        /*std::printf("pushing\n");
        for (auto b : bytes0)
            {
                std::printf("%d", b);
            }
            std::printf("\n");*/
    }
    std::printf("wrote %d blocks\n", blockcount);
}

void popfunc1()
{
    SharedBlockQueue queue = SharedBlockQueue(key, size, false);
    unsigned blockcount = 0;
    for (unsigned i = 0; i < (size / blocksize) * 100; i++) // force pop more than push times
    {
        std::vector<uint8_t> bytes1;
        if (queue.popblock(bytes1))
        {
            blockcount++;
            if(bytes1[0] != 6 && bytes1[bytes1.size() - 1] != 9)
            {
                throw;
            }
            /*for (auto b : bytes1)
            {
                std::printf("%d", b);
            }
            std::printf("\n");*/
        }
    }
    std::printf("popped %d blocks\n", blockcount);
}

void readfunc()
{
    SharedBlockQueue queue = SharedBlockQueue(key, size, false);
    unsigned blockcount = 0;
    for (unsigned i = 0; i < (size / blocksize) * 100; i++) // force pop more than push times
    {
        std::vector<uint8_t> bytes1;
        if (queue.readfront(bytes1))
        {
            blockcount++;
            for (auto b : bytes1)
            {
                std::printf("%d", b);
            }
            std::printf("\n");
        }
    }
    std::printf("read %d blocks\n", blockcount);
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        SharedBlockQueue queue = SharedBlockQueue(key, size, true);
/*begin singlethreaded test, dont run with other tests since they are testing with diffferent sizes*/
#if 0
        std::vector<uint8_t> bytes0(5, 0);
        std::vector<uint8_t> bytes1(5, 0);
        std::vector<uint8_t> bytes2(5, 0);
        std::vector<uint8_t> bytes3(5, 0);

        int counter = 0;
        for (unsigned i = 0; i < 5; i++)
        {
            bytes0[i] = counter;
            counter++;
        }

        for (unsigned i = 0; i < 5; i++)
        {
            bytes1[i] = counter;
            counter++;
        }

        for (unsigned i = 0; i < 5; i++)
        {
            bytes2[i] = counter;
            counter++;
        }

        for (unsigned i = 0; i < 5; i++)
        {
            bytes3[i] = counter;
            counter++;
        }

        queue.writeblock(bytes0);
        queue.writeblock(bytes1);
        queue.writeblock(bytes2);
        queue.writeblock(bytes3);

        /*this should fail*/
        std::printf("writing overflowret val %d\n", queue.writeblock(bytes3));

        std::vector<uint8_t> readbytes0(5, 0);
        std::vector<uint8_t> readbytes1(5, 0);
        std::vector<uint8_t> readbytes2(5, 0);
        std::vector<uint8_t> readbytes3(5, 0);

        std::cout << "reading back buffer\n";
        SharedBlockQueue readqueue = SharedBlockQueue(key, size, false);
        // readqueue.readfront(readbytes0);
        readqueue.popblock(readbytes0);
        // readqueue.readfront(readbytes1);
        readqueue.popblock(readbytes1);
        // readqueue.readfront(readbytes2);
        readqueue.popblock(readbytes2);
        // readqueue.readfront(readbytes3);
        readqueue.popblock(readbytes3);

        for (unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes0[i] << "\n";
        }

        for (unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes1[i] << "\n";
        }

        for (unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes2[i] << "\n";
        }

        for (unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes3[i] << "\n";
        }

        std::printf("queue should be empty: %d\n", readqueue.empty());
#endif
        /*begin multi process test*/
        std::string arg = argv[0];
        std::thread t1process(syscall0, arg);
        std::thread t2process(syscall1, arg);
        std::thread t3process(syscall2, arg);
        std::thread t4process(syscall3, arg);
        t1process.join();
        t2process.join();
        t3process.join();
        t4process.join();

        queue.releaseBuffer();
    }
    else if (argc == 2)
    {
        pushfunc();
    }
    else if (argc == 3)
    {
        readfunc();
    }
    else if (argc == 4)
    {
        popfunc1();
    }
    else if (argc == 5)
    {
        popfunc1();
    }
    return 0;
}