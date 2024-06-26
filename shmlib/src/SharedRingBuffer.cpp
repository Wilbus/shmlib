#include "SharedRingBuffer.h"

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

SharedRingBuffer::SharedRingBuffer()
{
}

SharedRingBuffer::SharedRingBuffer(key_t id, uint64_t size, bool newMem)
    : id(id)
    , master(newMem)
{
    shm = SharedMemory<uint8_t>(std::to_string(id), sizeof(uint8_t) * size, newMem);
    buffPropertiesShm = SharedMemory<uint64_t>(std::to_string(id + 1), sizeof(uint64_t) * PROPERTIES, newMem);
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

SharedRingBuffer::~SharedRingBuffer()
{
    // shm.markForRelease();
}

// remap the pointer addresses rather than copy the sharememory class' pointers
SharedRingBuffer::SharedRingBuffer(const SharedRingBuffer& other)
{
    id = other.getKey();
    master = other.getMaster();
    auto otherSize = other.getSize(); // in bytes

    shm = SharedMemory<uint8_t>(std::to_string(id), otherSize, master);
    buffPropertiesShm = SharedMemory<uint64_t>(std::to_string(id + 1), sizeof(uint64_t) * PROPERTIES, master);
    buffPtr = shm.get();
    propertiesPtr = buffPropertiesShm.get();
    buffPtr = shm.get();
}

// remap the pointer addresses rather than copy the sharememory class' pointers
SharedRingBuffer& SharedRingBuffer::operator=(const SharedRingBuffer& other)
{
    if (this == &other)
    {
        return *this;
    }

    id = other.getKey();
    master = other.getMaster();
    auto otherSize = other.getSize(); // in bytes

    shm = SharedMemory<uint8_t>(std::to_string(id), otherSize, master);
    buffPropertiesShm = SharedMemory<uint64_t>(std::to_string(id + 1), sizeof(uint64_t) * PROPERTIES, master);
    buffPtr = shm.get();
    propertiesPtr = buffPropertiesShm.get();
    buffPtr = shm.get();

    return *this;
}

bool SharedRingBuffer::push(uint8_t data)
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
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

bool SharedRingBuffer::pop()
{
    // std::cout << std::this_thread::get_id() << "pop() wait mutex\n";
    SharedSemaphoreSentry ss(std::to_string(id), true);
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

size_t SharedRingBuffer::size()
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _size - _head - _tail;
}

bool SharedRingBuffer::isEmpty()
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _opcount == 0;
}

uint8_t SharedRingBuffer::front()
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    uint8_t data = buffPtr[_head];
    return data;
}

bool SharedRingBuffer::releaseBuffer()
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    shm.markForRelease();
    buffPropertiesShm.markForRelease();
    return true;
}

uint64_t SharedRingBuffer::getIdx() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _tail;
}

uint64_t SharedRingBuffer::getTailIdx() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _tail;
}

size_t SharedRingBuffer::getSize() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _size;
}

SharedMemory<uint8_t> SharedRingBuffer::getShm() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return shm;
}

key_t SharedRingBuffer::getKey() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return id;
}

uint64_t SharedRingBuffer::getHeadIdx() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _head;
}

bool SharedRingBuffer::getMaster() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return master;
}

uint64_t SharedRingBuffer::getOpCount() const
{
    SharedSemaphoreSentry ss(std::to_string(id), true);
    return _opcount;
}
} // namespace shm