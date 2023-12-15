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
    sRingBuffer = SharedRingBufferNotThreadSafe(id, length, newMem);
    /*shm = SharedMemory<uint8_t>(std::to_string(id), sizeof(uint8_t) * length, newMem);
    buffPropertiesShm = SharedMemory<uint64_t>(std::to_string(id + 1), sizeof(uint64_t) * PROPERTIES, newMem);
    buffPtr = shm.get();
    propertiesPtr = buffPropertiesShm.get();

    if (newMem) // only init these to 0 if this is the first time opening this mem region
    {
        propertiesPtr[TAIL] = 0;
        propertiesPtr[HEAD] = 0;
        propertiesPtr[SIZE] = length; // number of bytes of shared memory, tail should not exceed length
        propertiesPtr[OPCOUNT] = 0;
    }*/
}

// SharedBuffer::~SharedBuffer()
//{
// shm.markForRelease();
//}

 bool SharedBuffer::releaseBuffer()
    {
        sRingBuffer.releaseBuffer();
    }

bool SharedBuffer::writeblock(std::vector<uint8_t> bytes)
{
    SharedSemaphoreSentry ss(std::to_string(id + 1), true);

    auto ringbufferOpCount = sRingBuffer.getOpCount(); // full when opcounet == size
    auto size = sRingBuffer.getSize();

    if (ringbufferOpCount + bytes.size() > size) // full
    {
        return false;
    }

    for (auto& byte : bytes)
    {
        sRingBuffer.push(byte);
    }
    return true;
}

bool SharedBuffer::popblock(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(std::to_string(id + 1), true);

    //we cant the block if the block size we are trying to pop is 
    //greater than the number of bytes currently in the ring buffer
    auto ringbufferOpCount = sRingBuffer.getOpCount(); 
    if(bytes.size() > ringbufferOpCount || empty())
    {
        return false;
    }

    for(unsigned i = 0; i < bytes.size(); i++)
    {
        bytes[i] = sRingBuffer.front();
        sRingBuffer.pop();
    }
    return true;
}

bool SharedBuffer::readfront(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(std::to_string(id + 1), true);

    if(empty())
    {
        return false;
    }
    
    for(unsigned i = 0; i < bytes.size(); i++)
    {
        bytes[i] = sRingBuffer.front();
        sRingBuffer.pop();
    }
    return true;
}

bool SharedBuffer::empty()
{
    return sRingBuffer.isEmpty();
}

} // namespace shm