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
    bool empty();

    bool releaseBuffer();

private:
    /*key_t id;
    bool master;
    uint8_t* buffPtr;
    SharedMemory<uint8_t> shm;
    SharedMemory<uint64_t> buffPropertiesShm;
    uint64_t* propertiesPtr;*/
    key_t id;
    bool master;
    SharedRingBufferNotThreadSafe sRingBuffer;
};
} // namespace shm