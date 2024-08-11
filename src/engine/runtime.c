#include <core/definitions.h>
#include <core/arena.h>

static memory_arena primary_arena;

b32 
runtime_init(buffer heap)
{

    // Rather than dealing with the raw heap buffer, convert it to a memory arena.
    memory_arena_initialize(&primary_arena, heap.ptr, heap.size);

    // Return true to indicate that init succeeded.
    return true;

}

b32 
runtime_main(buffer heap)
{

    while (true)
    {

    }


    // Since runtime_main is a proxy for main, we return 0 for success here.
    return 0;

}

