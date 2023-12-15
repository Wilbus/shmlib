#include "SharedMemory.h"

using namespace shm;

int main(int argc, char* argv[])
{
    size_t size = 256;
    if (argc == 1)
    {
        SharedMemory<uint8_t> sharedmem("mem0", sizeof(uint8_t) * size, true);
        auto ptr = sharedmem.get();

        for (unsigned i = 0; i < size; i++)
        {
            ptr[i] = i;
        }

        // Launch child process
        std::string s(argv[0]);
        s += " child ";
        if (0 != std::system(s.c_str()))
            return 1;
    }
    else
    {
        SharedMemory<uint8_t> sharedmem("mem0", sizeof(uint8_t) * size, true);
        auto ptr = sharedmem.get();

        for (unsigned i = 0; i < size; i++)
        {
            std::cout << (unsigned)ptr[i] << "\n";
        }
    }
    return 0;
}