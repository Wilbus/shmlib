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
}

// SharedBuffer::~SharedBuffer()
//{
// shm.markForRelease();
//}

bool SharedBuffer::releaseBuffer()
{
    SharedSemaphoreSentry ss(std::to_string(id + 1), true);
    sRingBuffer.releaseBuffer();
    return true;
}

bool SharedBuffer::writeblock(std::vector<uint8_t> bytes)
{
    SharedSemaphoreSentry ss(std::to_string(id + 1));

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
    SharedSemaphoreSentry ss(std::to_string(id + 1));

    // we cant pop the block if the block size we are trying to pop is
    // greater than the number of bytes currently in the ring buffer
    auto ringbufferOpCount = sRingBuffer.getOpCount();
    if (bytes.size() > ringbufferOpCount || empty())
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

bool SharedBuffer::readfront(std::vector<uint8_t>& bytes)
{
    SharedSemaphoreSentry ss(std::to_string(id + 1));

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