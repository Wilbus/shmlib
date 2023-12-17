#include "SharedBlockQueue.h"

#include <iostream>
#include <unistd.h>

namespace shm {

// we use string type of key id to identify memory region
SharedBlockQueue::SharedBlockQueue()
{
}

SharedBlockQueue::SharedBlockQueue(key_t id, uint64_t length, bool newMem)
    : id(id)
    , master(newMem)
    , length(length)
{
    semaphoreKeyName = std::to_string(id) + semaphoreName;
    sRingBuffer = SharedRingBufferNotThreadSafe(id, length, newMem);
    sSizesBuffer = TSharedRingBufferNotThreadSafe<uint64_t>(id, length, newMem);
}

SharedBlockQueue::~SharedBlockQueue()
{
    // shm.markForRelease();
    auto pid = getpid();
    std::printf("%d: SharedBlockQueue destructor\n", pid);
}

bool SharedBlockQueue::releaseBuffer()
{
    // open semaphore as parent so it gets deleted on exit of this function
    SharedSemaphoreSentry ss(semaphoreKeyName, true);
    sRingBuffer.releaseBuffer();
    sSizesBuffer.releaseBuffer();
    return true;
}

bool SharedBlockQueue::writeblock(std::vector<uint8_t> bytes)
{
    SharedSemaphoreSentry ss(semaphoreKeyName);

    if (!pushblocksize(bytes.size()))
    {
        return false; // queue holding sizes of blocks is full, so don't push anymore bytes into blocks queue
    }

    auto ringbufferOpCount = sRingBuffer.getOpCount(); // full when opcounet == size
    auto size = sRingBuffer.getSize();

    if (ringbufferOpCount + bytes.size() >
        size) // the block we are trying to push exceeds the remaining bytes in buffer queue
    {
        // throw std::runtime_error("mismatch in size queue and bufferqueue when pushing");
        return false;
    }

    for (auto& byte : bytes)
    {
        sRingBuffer.push(byte);
    }
    return true;
}

bool SharedBlockQueue::pushblocksize(uint64_t size)
{
    if (!sSizesBuffer.push(size))
    {
        return false;
    }
    return true;
}

bool SharedBlockQueue::popblock()
{
    SharedSemaphoreSentry ss(semaphoreKeyName);

    uint64_t blockSizeAtHead = 0;
    if (!popblocksize(blockSizeAtHead))
    {
        return false;
    }

    // we cant pop the block if the block size we are trying to pop is
    // greater than the number of bytes currently in the ring buffer
    auto opcount = sRingBuffer.getOpCount();
    if (blockSizeAtHead > opcount || empty())
    {
        throw std::runtime_error("mismatch in size queue and bufferqueue when popping");
    }

    for (unsigned i = 0; i < blockSizeAtHead; i++)
    {
        sRingBuffer.pop();
    }

    return true;
}

bool SharedBlockQueue::popblock(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(semaphoreKeyName);

    uint64_t blockSizeAtHead = 0;
    if (!popblocksize(blockSizeAtHead))
    {
        return false;
    }

    // we cant pop the block if the block size we are trying to pop is
    // greater than the number of bytes currently in the ring buffer
    auto opcount = sRingBuffer.getOpCount();
    if (blockSizeAtHead > opcount || empty())
    {
        throw std::runtime_error("mismatch in size queue and bufferqueue when popping");
    }

    bytes.resize(blockSizeAtHead);
    for (unsigned i = 0; i < blockSizeAtHead; i++)
    {
        bytes[i] = sRingBuffer.front();
        sRingBuffer.pop();
    }

    return true;
}

bool SharedBlockQueue::popblocksize(uint64_t& size)
{
    if (!sSizesBuffer.pop(size))
    {
        return false;
    }
    return true;
}

bool SharedBlockQueue::readfront(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(semaphoreKeyName);

    if (empty())
    {
        return false;
    }

    uint64_t blockSizeAtHead = 0;
    if (!readfrontsize(blockSizeAtHead)) // false if empty
    {
        return false;
    }

    auto opcount = sRingBuffer.getOpCount();
    if (blockSizeAtHead > opcount || empty())
    {
        throw std::runtime_error("mismatch in size queue and bufferqueue when reading front");
    }

    bytes.resize(blockSizeAtHead);

    auto head = sRingBuffer.getHeadIdx();
    auto size = sRingBuffer.getSize();
    for (unsigned i = 0; i < blockSizeAtHead; i++)
    {
        bytes[i] = sRingBuffer.getBuffPtr()[head];
        head = (head + 1) % size;
    }
    return true;
}

bool SharedBlockQueue::readfrontsize(uint64_t& size)
{
    if (!sSizesBuffer.isEmpty())
    {
        size = sSizesBuffer.front();
        return true;
    }
    return false;
}

bool SharedBlockQueue::empty()
{
    // dont acquire lock here, for internal class use only for now
    return sRingBuffer.isEmpty();
}

} // namespace shm