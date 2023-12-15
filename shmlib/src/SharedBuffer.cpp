#pragma once

#include "SharedBuffer.h"

namespace shm {

// we use string type of key id to identify memory region
SharedBuffer::SharedBuffer()
{
}

SharedBuffer::SharedBuffer(key_t id, uint64_t length, bool newMem)
    : id(id)
    , master(newMem)
{
    shm = SharedMemory<uint8_t>(std::to_string(id), sizeof(uint8_t) * length, newMem);
    buffPropertiesShm = SharedMemory<uint64_t>(std::to_string(id + 1), sizeof(uint64_t) * PROPERTIES, newMem);
    buffPtr = shm.get();
    propertiesPtr = buffPropertiesShm.get();

    if (newMem) // only init these to 0 if this is the first time opening this mem region
    {
        propertiesPtr[TAIL] = 0;
        propertiesPtr[HEAD] = 0;
        propertiesPtr[SIZE] = length; // number of bytes of shared memory, tail should not exceed length
        propertiesPtr[OPCOUNT] = 0;
    }
}

SharedBuffer::~SharedBuffer()
{
    shm.markForRelease();
}

/*
Write block of data to shared memory
 - check if block of data exceeds memory region through tail index
 - if not, copy bytes into memory region starting at tail index
*/
bool SharedBuffer::writeblock(std::vector<uint8_t> bytes)
{
    SharedSemaphoreSentry ss(std::to_string(id), true);

    if (_tail + bytes.size() > _size - 1)
    {
        return false;
    }

    // for(uint64_t currIdx = _tail; currIdx < _tail + bytes.size(); currIdx++)
    //{
    std::copy(bytes.begin(), bytes.end(), &buffPtr[_tail]);
    _tail += bytes.size();
    //}
}

bool SharedBuffer::readblock(std::vector<uint8_t>& bytes, size_t size)
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
}

bool SharedBuffer::readfront(std::vector<uint8_t>& bytes, size_t size)
{
}

} // namespace shm