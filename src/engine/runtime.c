#include <core/definitions.h>
#include <core/arena.h>

static memory_arena primary_arena;

b32 
runtime_init(buffer heap)
{

    // Rather than dealing with the raw heap buffer, convert it to a memory arena.
    memory_arena_initialize(&primary_arena, heap.ptr, heap.size);

    // Create the window, automatically show it to the user after it is made.
    b32 window_created = window_initialize("Ninetails Game Engine", 1280, 720, true);
    if (window_created == false) return false;

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

