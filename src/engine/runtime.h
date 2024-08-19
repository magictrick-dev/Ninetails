#ifndef SRC_ENGINE_RUNTIME_H
#define SRC_ENGINE_RUNTIME_H
#include <core/definitions.h>
#include <core/arena.h>

b32 runtime_init(buffer heap);
b32 runtime_main(buffer heap);
memory_arena* runtime_get_primary_arena();

#endif
