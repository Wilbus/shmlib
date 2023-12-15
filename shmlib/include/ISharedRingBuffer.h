#pragma once
#include "SharedMemory.h"

#include <cstdlib>
#include <stdint.h>
namespace shm {

class ISharedRingBuffer
{
public:
    virtual bool push(uint8_t data) = 0;
    virtual bool pop() = 0;
    virtual size_t size() = 0;
    virtual bool isEmpty() = 0;
    virtual uint8_t front() = 0;
    virtual uint64_t getIdx() const = 0;
    virtual size_t getSize() const = 0;
    virtual SharedMemory<uint8_t> getShm() const = 0;
    virtual key_t getKey() const = 0;
};
} // namespace shm