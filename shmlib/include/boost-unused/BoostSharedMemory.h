#pragma once
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <map>
#include <vector>
namespace shm {

#define MESSAGES_BUFFER_REGION_KEY "messages"
static const std::map<std::string, std::string> memory_regions = {{MESSAGES_BUFFER_REGION_KEY, "message_region"}};

using namespace boost::interprocess;

#define SHAREDSIZE 16000000000

// Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
// This allocator will allow placing containers in the segment
typedef allocator<uint8_t, managed_shared_memory::segment_manager> ShmemAllocator;

// Alias a vector that uses the previous STL-like allocator so that allocates
// its values from the segment
typedef vector<uint8_t, ShmemAllocator> myvector;

class BoostSharedMemory
{
public:
    struct shm_remove
    {
        shm_remove()
        {
            shared_memory_object::remove("MySharedMemory");
        }
        ~shm_remove()
        {
            shared_memory_object::remove("MySharedMemory");
        }
    } remover;

    BoostSharedMemory(bool parent)
        : segment(managed_shared_memory(create_only, "MySharedMemory", SHAREDSIZE))
        , alloc_inst(ShmemAllocator(segment.get_segment_manager()))
    {
        // Create a new segment with given name and size
        /*if(parent)
        {
            segment = managed_shared_memory(create_only, "MySharedMemory", SHAREDSIZE);
            // Initialize shared memory STL-compatible allocator
            alloc_inst = ShmemAllocator(segment.get_segment_manager());
        }
        else
        {
             segment = managed_shared_memory(open_only, "MySharedMemory");
            // Initialize shared memory STL-compatible allocator
            alloc_inst = ShmemAllocator(segment.get_segment_manager());
        }*/
    }

    BoostSharedMemory()
        : segment(managed_shared_memory(open_only, "MySharedMemory"))
        , alloc_inst(ShmemAllocator(segment.get_segment_manager()))
    {
    }

    ~BoostSharedMemory()
    {
        segment.destroy<myvector>("MyVector");
    }

    myvector* segmentConstruct()
    {
        return segment.construct<myvector>("MyVector")(alloc_inst);
    }

    myvector* segmentFind()
    {
        return segment.find<myvector>("MyVector").first;
    }

    bool isParent() const
    {
        return parent;
    }

private:
    std::string regionName;
    managed_shared_memory segment;
    myvector* region;
    ShmemAllocator alloc_inst;
    bool parent{false};
};
} // namespace shm