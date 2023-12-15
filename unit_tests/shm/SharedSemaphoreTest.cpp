#include "SharedSemaphore.h"
#include "SharedSemaphoreSentry.h"

#include <cstdlib> //std::system
#include <iostream>
#include <string>
#include <thread>

using namespace shm;

std::string sem0 = "sem0";
std::string sem1 = "sem1";

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
    s += " 2 3";
    std::cout << "start t2\n";
    if (0 != std::system(s.c_str()))
    {
    }
}

void syscall2(std::string arg)
{
    // Launch child process
    std::string s(arg);
    s += " 2 3 4";
    std::cout << "start t3\n";
    if (0 != std::system(s.c_str()))
    {
    }
}

void sentry1func()
{
    // std::cout << "sentryfunc1() start\n";
    SharedSemaphoreSentry ss1(sem1, true);
    std::cout << "sentryfunc1() start\n";
    for (unsigned i = 0; i < 1000000; i++)
    {
        std::cout << i << "\n";
    }
    std::cout << "sentryfunc1() end\n";
}

void sentry2func()
{
    std::cout << "sentryfunc2() start\n";
    SharedSemaphoreSentry ss1(sem1, true);
    std::cout << "sentryfunc2() end\n";
}

void sentry3func()
{
    std::cout << "sentryfunc3() start\n";
    SharedSemaphoreSentry ss1(sem1, true);
    std::cout << "sentryfunc3() end\n";
}

void outterfunc()
{
    sentry1func();
}

int main(int argc, char* argv[])
{
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

        // std::thread t1process(syscall0, arg);
        // std::thread t2process(syscall1, arg);
        // std::thread t3process(syscall2, arg);

        std::thread t1process(syscall0, arg);
        std::thread t2process(syscall0, arg);
        std::thread t3process(syscall0, arg);

        t1process.join();
        t2process.join();
        t3process.join();
    }
    else if (argc == 2)
    {
        // sentry1func();
        outterfunc();
    }
    else if (argc == 3)
    {
        // sentry2func();
        outterfunc();
    }
    else if (argc == 4)
    {
        // sentry3func();
        outterfunc();
    }
    return 0;
}