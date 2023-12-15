#include "SharedBuffer.h"
#include <cassert>
#include <stdio.h>
#include <unistd.h>

using namespace shm;

    size_t size = 100000;
    key_t key = 1234;

    size_t blocksize = 10;

    unsigned counter1 = 0;

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
    SharedBuffer queue = SharedBuffer(key, size, false);

    unsigned blockcount = 0;
    for(unsigned i = 0; i < size / blocksize; i++) //push size / vecsize blocks
    {
        std::vector<uint8_t> bytes0(blocksize, 0);
        int counter = 0;
        for(unsigned i = 0; i < blocksize; i++)
        {
            bytes0[i] = counter;
            counter++;
        }
        queue.writeblock(bytes0);
        blockcount++;
    }
    std::printf("wrote %d blocks\n", blockcount);
}

void popfunc1()
{
    SharedBuffer queue = SharedBuffer(key, size, false);
    unsigned blockcount = 0;
    for(unsigned i = 0; i < (size / blocksize) * 100; i++) //force pop more than push times
    {
        std::vector<uint8_t> bytes1(blocksize, 0);
        if(queue.popblock(bytes1))
        {
            blockcount++;
        }
    }
    std::printf("read %d blocks\n", blockcount);
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        SharedBuffer queue = SharedBuffer(key, size, true);
        std::vector<uint8_t> bytes0(5, 0);
        std::vector<uint8_t> bytes1(5, 0);
        std::vector<uint8_t> bytes2(5, 0);
        std::vector<uint8_t> bytes3(5, 0);

        int counter = 0;
        for(unsigned i = 0; i < 5; i++)
        {
            bytes0[i] = counter;
            counter++;
        }

        for(unsigned i = 0; i < 5; i++)
        {
            bytes1[i] = counter;
            counter++;
        }

        for(unsigned i = 0; i < 5; i++)
        {
            bytes2[i] = counter;
            counter++;
        }

        for(unsigned i = 0; i < 5; i++)
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
        SharedBuffer readqueue = SharedBuffer(key, size, false);
        //readqueue.readfront(readbytes0);
        readqueue.popblock(readbytes0);
        //readqueue.readfront(readbytes1);
        readqueue.popblock(readbytes1);
        //readqueue.readfront(readbytes2);
        readqueue.popblock(readbytes2);
        //readqueue.readfront(readbytes3);
        readqueue.popblock(readbytes3);

        for(unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes0[i] << "\n";
        }

        for(unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes1[i] << "\n";
        }

        for(unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes2[i] << "\n";
        }

        for(unsigned i = 0; i < 5; i++)
        {
            std::cout << (unsigned)readbytes3[i] << "\n";
        }

        std::printf("queue should be empty: %d\n", readqueue.empty());

/*begin multi process test*/
        std::string arg = argv[0];
        std::thread t1process(syscall0, arg);
        std::thread t2process(syscall1, arg);
        std::thread t3process(syscall2, arg);
        t1process.join();
        t2process.join();
        t3process.join();

        readqueue.releaseBuffer();
    }
    else if (argc == 2)
    {
        pushfunc();
    }
    else if (argc == 3)
    {
        popfunc1();
    }
    else if (argc == 4)
    {
        popfunc1();
    }
    return 0;
}