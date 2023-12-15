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
    semaphoreKeyName = std::to_string(id) + semaphoreName;
    sRingBuffer = SharedRingBufferNotThreadSafe(id, length, newMem);
    sSizesBuffer = TSharedRingBufferNotThreadSafe<uint64_t>(id, length, newMem);
}

// SharedBuffer::~SharedBuffer()
//{
// shm.markForRelease();
//}

bool SharedBuffer::releaseBuffer()
{
    // open semaphore as parent so it gets deleted on exit of this function
    SharedSemaphoreSentry ss(semaphoreKeyName, true);
    sRingBuffer.releaseBuffer();
    sSizesBuffer.releaseBuffer();
    return true;
}

bool SharedBuffer::writeblock(std::vector<uint8_t> bytes)
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
        throw std::runtime_error("mismatch in size queue and bufferqueue when pushing");
    }

    for (auto& byte : bytes)
    {
        sRingBuffer.push(byte);
    }
    return true;
}

bool SharedBuffer::pushblocksize(uint64_t size)
{
    if (!sSizesBuffer.push(size))
    {
        return false;
    }
    return true;
}

bool SharedBuffer::popblock(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(semaphoreKeyName);

    uint64_t blockSizeAtHead = 0;
    if (!popblocksize(blockSizeAtHead))
    {
        // std::string msg = "ppblock() requested bytes: " + std::to_string(bytes.size()) + ", curr opcount " +
        // std::to_string(ringbufferOpCount) + "\n"; std::cout << msg;
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

bool SharedBuffer::popblocksize(uint64_t& size)
{
    if (!sSizesBuffer.pop(size))
    {
        return false;
    }
    return true;
}

bool SharedBuffer::readfront(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(semaphoreKeyName);

    if (empty())
    {
        return false;
    }

    for (unsigned i = 0; i < bytes.size(); i++)
    {
        bytes[i] = sRingBuffer.front();
        sRingBuffer.pop();
    }
    return true;
}

bool SharedBuffer::empty()
{
    // dont acquire lock here, for internal class use only for now
    return sRingBuffer.isEmpty();
}

} // namespace shm