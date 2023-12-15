#pragma once

#include "SharedMemory.h"
#include "SharedSemaphoreSentry.h"

#include <vector>

namespace shm {

class SharedBuffer
{
public:
    SharedBuffer();

    SharedBuffer(key_t id, uint64_t length, bool newMem);

    ~SharedBuffer();

    bool writeblock(std::vector<uint8_t> bytes);
    bool readblock(std::vector<uint8_t>& bytes, size_t size);
    bool readfront(std::vector<uint8_t>& bytes, size_t size);

private:
    key_t id;
    bool master;
    uint8_t* buffPtr;
    SharedMemory<uint8_t> shm;
    SharedMemory<uint64_t> buffPropertiesShm;
    uint64_t* propertiesPtr;
};
} // namespace shm