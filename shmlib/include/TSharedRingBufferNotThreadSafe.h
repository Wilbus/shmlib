#pragma once

#include "ISharedRingBuffer.h"
#include "SharedMemory.h"
#include "SharedSemaphoreSentry.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

namespace shm {

template <typename T>
class TSharedRingBufferNotThreadSafe
{
public:
    const uint8_t PROPERTIES = 4;
    const uint8_t TAIL = 0;
    const uint8_t HEAD = 1;
    const uint8_t SIZE = 2;
    const uint8_t OPCOUNT = 3;

#define _tail propertiesPtr[TAIL]
#define _head propertiesPtr[HEAD]
#define _size propertiesPtr[SIZE]
#define _opcount propertiesPtr[OPCOUNT]

    TSharedRingBufferNotThreadSafe()
    {
    }

    TSharedRingBufferNotThreadSafe(key_t id, uint64_t size, bool newMem)
        : id(id)
        , master(newMem)
    {
        std::string name = std::to_string(id) + memRegionName;
        std::string propname = std::to_string(id) + propertiesRegionName;

        shm = SharedMemory<T>(name, sizeof(T) * size, newMem);
        buffPropertiesShm = SharedMemory<uint64_t>(propname, sizeof(uint64_t) * PROPERTIES, newMem);
        buffPtr = shm.get();
        propertiesPtr = buffPropertiesShm.get();
        buffPtr = shm.get();
        if (newMem) // only init these to 0 if this is the first time opening this mem region
        {
            propertiesPtr[TAIL] = 0;
            propertiesPtr[HEAD] = 0;
            propertiesPtr[SIZE] = size; // number of bytes of shared memory, tail should not exceed length
            propertiesPtr[OPCOUNT] = 0;
        }
    }

    ~TSharedRingBufferNotThreadSafe()
    {
        // shm.markForRelease();
    }

    // remap the pointer addresses rather than copy the sharememory class' pointers
    TSharedRingBufferNotThreadSafe(const TSharedRingBufferNotThreadSafe& other)
    {
        id = other.getKey();
        master = other.getMaster();
        auto otherSize = other.getSize(); // in bytes

        std::string name = std::to_string(id) + memRegionName;
        std::string propname = std::to_string(id) + propertiesRegionName;

        shm = SharedMemory<T>(name, otherSize, master);
        buffPropertiesShm = SharedMemory<uint64_t>(propname, sizeof(uint64_t) * PROPERTIES, master);
        buffPtr = shm.get();
        propertiesPtr = buffPropertiesShm.get();
        buffPtr = shm.get();
    }

    // remap the pointer addresses rather than copy the sharememory class' pointers
    TSharedRingBufferNotThreadSafe& operator=(const TSharedRingBufferNotThreadSafe& other)
    {
        if (this == &other)
        {
            return *this;
        }

        id = other.getKey();
        master = other.getMaster();
        auto otherSize = other.getSize(); // in bytes

        std::string name = std::to_string(id) + memRegionName;
        std::string propname = std::to_string(id) + propertiesRegionName;

        shm = SharedMemory<T>(name, otherSize, master);
        buffPropertiesShm = SharedMemory<uint64_t>(propname, sizeof(uint64_t) * PROPERTIES, master);
        buffPtr = shm.get();
        propertiesPtr = buffPropertiesShm.get();
        buffPtr = shm.get();

        return *this;
    }

    bool push(T data)
    {
        if (_opcount == _size)
        {
            // buffer is full
            // throw std::runtime_error("buffer is full"); //throw for now
            return false;
        }
        buffPtr[_tail] = data;
        _tail = (_tail + 1) % _size;
        _opcount++;
        return true;
    }

    bool pop(T& frontval)
    {
        // std::cout << std::this_thread::get_id() << "pop() wait mutex\n";

        // if ((_head + 1) % _size > _tail)
        if (_opcount == 0)
        {
            // std::cout << std::this_thread::get_id() << "error done mutex\n";
            /*don't throw. its possible that t1 is waiting at lock_guard, and t2
            had just decremented _opcount. when t2 exists pop() t1 will see _opcount == 0 and throws*/
            // throw std::runtime_error("pop error");
            return false;
        }
        // std::cout << _opcount << " " << _head << "\n";
        frontval = buffPtr[_head];
        _head = (_head + 1) % _size;
        _opcount--;
        // std::cout << std::this_thread::get_id() << "pop() done mutex opcount " << _opcount << "\n";
        return true;
    }

    size_t size()
    {
        return _size - _head - _tail;
    }

    bool isEmpty()
    {
        return _opcount == 0;
    }

    T front()
    {
        T data = buffPtr[_head];
        return data;
    }

    bool releaseBuffer()
    {
        shm.markForRelease();
        buffPropertiesShm.markForRelease();
        return true;
    }

    uint64_t getIdx() const
    {
        return _tail;
    }

    uint64_t getTailIdx() const
    {
        return _tail;
    }

    size_t getSize() const
    {
        return _size;
    }

    SharedMemory<T> getShm() const
    {
        return shm;
    }

    key_t getKey() const
    {
        return id;
    }

    uint64_t getHeadIdx() const
    {
        return _head;
    }

    T* getBuffPtr() const
    {
        return buffPtr;
    }

    bool getMaster() const
    {
        return master;
    }

    uint64_t getOpCount() const
    {
        return _opcount;
    }

private:
    key_t id;
    bool master;
    T* buffPtr;
    SharedMemory<T> shm;
    SharedMemory<uint64_t> buffPropertiesShm;
    uint64_t* propertiesPtr;
    const std::string memRegionName = "_TSharedRingBufferNotThreadSafe_memoryRegion";
    const std::string propertiesRegionName = "_TSharedRingBufferNotThreadSafe_propertiesRegion";
};
} // namespace shm