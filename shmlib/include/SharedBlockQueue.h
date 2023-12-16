#pragma once

#include "SharedRingBufferNotThreadSafe.h"
#include "SharedSemaphoreSentry.h"
#include "TSharedRingBufferNotThreadSafe.h"

#include <vector>

namespace shm {

/*
  class manages a block of memory as a queue for serialized data
  pushes and pops streams of bytes. we should know the size of
  the serialized data before hand, and this class probably
  won't work well if we try to store variable length data

- different regions of memory need to use key_t id's in increments of 2
  because the semaphore lock uses key_id id + 1

- write block of bytes to ring buffer
- read and pop block of bytes from ring buffer
*/

class SharedBlockQueue
{
public:
    SharedBlockQueue();

    SharedBlockQueue(key_t id, uint64_t length, bool newMem);

    //~SharedBlockQueue();

    bool writeblock(std::vector<uint8_t> bytes);
    bool popblock(std::vector<uint8_t>& bytes);
    bool popblock();
    bool readfront(std::vector<uint8_t>& bytes);

    bool releaseBuffer();

private:
    bool empty();
    bool pushblocksize(uint64_t size);
    bool popblocksize(uint64_t& size);
    bool readfrontsize(uint64_t& size);

    key_t id;
    bool master;
    std::string semaphoreKeyName;
    SharedRingBufferNotThreadSafe sRingBuffer;
    TSharedRingBufferNotThreadSafe<uint64_t> sSizesBuffer;
    const std::string semaphoreName = "_SharedVarBlockQueue_semaphore";
};
} // namespace shm