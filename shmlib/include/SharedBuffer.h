#pragma once

#include "SharedRingBufferNotThreadSafe.h"
#include "SharedSemaphoreSentry.h"

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

class SharedBuffer
{
public:
    SharedBuffer();

    SharedBuffer(key_t id, uint64_t length, bool newMem);

    //~SharedBuffer();

    bool writeblock(std::vector<uint8_t> bytes);
    bool popblock(std::vector<uint8_t>& bytes);
    bool readfront(std::vector<uint8_t>& bytes);

    bool releaseBuffer();

private:
    bool empty();
    key_t id;
    bool master;
    SharedRingBufferNotThreadSafe sRingBuffer;
};
} // namespace shm