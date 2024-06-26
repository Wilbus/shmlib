#include "SharedRingBufferNotThreadSafe.h"

#include "SharedSemaphoreSentry.h"
namespace shm {

const uint8_t PROPERTIES = 4;
const uint8_t TAIL = 0;
const uint8_t HEAD = 1;
const uint8_t SIZE = 2;
const uint8_t OPCOUNT = 3;

#define _tail propertiesPtr[TAIL]
#define _head propertiesPtr[HEAD]
#define _size propertiesPtr[SIZE]
#define _opcount propertiesPtr[OPCOUNT]

SharedRingBufferNotThreadSafe::SharedRingBufferNotThreadSafe()
{
}

SharedRingBufferNotThreadSafe::SharedRingBufferNotThreadSafe(key_t id, uint64_t size, bool newMem)
    : id(id)
    , master(newMem)
{
    std::string name = std::to_string(id) + memRegionName;
    std::string propname = std::to_string(id) + propertiesRegionName;

    shm = SharedMemory<uint8_t>(name, sizeof(uint8_t) * size, newMem);
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

SharedRingBufferNotThreadSafe::~SharedRingBufferNotThreadSafe()
{
    // shm.markForRelease();
}

// remap the pointer addresses rather than copy the sharememory class' pointers
SharedRingBufferNotThreadSafe::SharedRingBufferNotThreadSafe(const SharedRingBufferNotThreadSafe& other)
{
    id = other.getKey();
    master = other.getMaster();
    auto otherSize = other.getSize(); // in bytes

    std::string name = std::to_string(id) + memRegionName;
    std::string propname = std::to_string(id) + propertiesRegionName;

    shm = SharedMemory<uint8_t>(name, otherSize, master);
    buffPropertiesShm = SharedMemory<uint64_t>(propname, sizeof(uint64_t) * PROPERTIES, master);
    buffPtr = shm.get();
    propertiesPtr = buffPropertiesShm.get();
    buffPtr = shm.get();
}

// remap the pointer addresses rather than copy the sharememory class' pointers
SharedRingBufferNotThreadSafe& SharedRingBufferNotThreadSafe::operator=(const SharedRingBufferNotThreadSafe& other)
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

    shm = SharedMemory<uint8_t>(name, otherSize, master);
    buffPropertiesShm = SharedMemory<uint64_t>(propname, sizeof(uint64_t) * PROPERTIES, master);
    buffPtr = shm.get();
    propertiesPtr = buffPropertiesShm.get();
    buffPtr = shm.get();

    return *this;
}

bool SharedRingBufferNotThreadSafe::push(uint8_t data)
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

bool SharedRingBufferNotThreadSafe::pop()
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
    _head = (_head + 1) % _size;
    _opcount--;
    // std::cout << std::this_thread::get_id() << "pop() done mutex opcount " << _opcount << "\n";
    return true;
}

size_t SharedRingBufferNotThreadSafe::size()
{
    return _size - _head - _tail;
}

bool SharedRingBufferNotThreadSafe::isEmpty()
{
    return _opcount == 0;
}

uint8_t SharedRingBufferNotThreadSafe::front()
{
    uint8_t data = buffPtr[_head];
    return data;
}

bool SharedRingBufferNotThreadSafe::releaseBuffer()
{
    shm.markForRelease();
    buffPropertiesShm.markForRelease();
    return true;
}

uint64_t SharedRingBufferNotThreadSafe::getIdx() const
{
    return _tail;
}

uint64_t SharedRingBufferNotThreadSafe::getTailIdx() const
{
    return _tail;
}

size_t SharedRingBufferNotThreadSafe::getSize() const
{
    return _size;
}

SharedMemory<uint8_t> SharedRingBufferNotThreadSafe::getShm() const
{
    return shm;
}

key_t SharedRingBufferNotThreadSafe::getKey() const
{
    return id;
}

uint64_t SharedRingBufferNotThreadSafe::getHeadIdx() const
{
    return _head;
}

bool SharedRingBufferNotThreadSafe::getMaster() const
{
    return master;
}

uint64_t SharedRingBufferNotThreadSafe::getOpCount() const
{
    return _opcount;
}

uint8_t* SharedRingBufferNotThreadSafe::getBuffPtr() const
{
    return buffPtr;
}
} // namespace shm