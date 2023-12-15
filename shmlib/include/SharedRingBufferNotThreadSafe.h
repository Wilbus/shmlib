#pragma once

#include "ISharedRingBuffer.h"
#include "SharedMemory.h"

#include <mutex>
#include <thread>

namespace shm {

class SharedRingBufferNotThreadSafe : public ISharedRingBuffer
{
public:
    SharedRingBufferNotThreadSafe();

    SharedRingBufferNotThreadSafe(key_t id, uint64_t length, bool newMem);

    ~SharedRingBufferNotThreadSafe();

    virtual bool push(uint8_t data) override;

    virtual bool pop() override;

    virtual size_t size() override;

    virtual bool isEmpty() override;

    virtual uint8_t front() override;

    virtual uint64_t getIdx() const override;

    virtual size_t getSize() const override;

    virtual SharedMemory<uint8_t> getShm() const override;

    virtual key_t getKey() const override;

    uint64_t getHeadIdx() const;

    bool releaseBuffer();

    uint64_t getTailIdx() const;

    bool getMaster() const;

    uint64_t getOpCount() const;

    uint8_t* getBuffPtr() const;

    SharedRingBufferNotThreadSafe(const SharedRingBufferNotThreadSafe& other);

    SharedRingBufferNotThreadSafe& operator=(const SharedRingBufferNotThreadSafe& other);

private:
    key_t id;
    bool master;
    uint8_t* buffPtr;
    SharedMemory<uint8_t> shm;
    SharedMemory<uint64_t> buffPropertiesShm;
    uint64_t* propertiesPtr;
    const std::string memRegionName = "_SharedRingBufferNotThreadSafe_memoryRegion";
    const std::string propertiesRegionName = "_SharedRingBufferNotThreadSafe_propertiesRegion";
};
} // namespace shm