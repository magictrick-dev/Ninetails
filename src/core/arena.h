#ifndef SRC_CORE_ARENA_H
#define SRC_CORE_ARENA_H
#include <core/definitions.h>

#define memory_arena_push_type(arena, type) (type*)memory_arena_push(arena, sizeof(type))
#define memory_arena_push_type_top(arena, type) (type*)memory_arena_push_top(arena, sizeof(type))
#define memory_arena_push_array(arena, type, count) (type*)memory_arena_push(arena, sizeof(type)*(count))
#define memory_arena_push_array_top(arena, type, count) (type*)memory_arena_push_top(arena, sizeof(type)*(count))

#define memory_arena_pop_type(arena, type) memory_arena_pop(arena, sizeof(type))
#define memory_arena_pop_type_top(arena, type) memory_arena_pop_top(arena, sizeof(type))
#define memory_arena_pop_array(arena, type, count) memory_arena_pop(arena, sizeof(type)*(count))
#define memory_arena_pop_array_top(arena, type, count) memory_arena_pop_top(arena, sizeof(type)*(count))

typedef struct memory_arena
{
    vptr buffer;
    u64 size;
    u64 commit_bottom;
    u64 commit_top;
} memory_arena;

void        memory_arena_initialize(memory_arena *arena, vptr buffer, u64 size);

void*       memory_arena_push(memory_arena *arena, u64 size);
void        memory_arena_pop(memory_arena *arena, u64 size);
void        memory_arena_partition(memory_arena *parent, memory_arena *child, u64 size);
u64         memory_arena_save(memory_arena *arena);
void        memory_arena_restore(memory_arena *arena, u64 state);

void*       memory_arena_push_top(memory_arena *arena, u64 size);
void        memory_arena_pop_top(memory_arena *arena, u64 size);
void        memory_arena_partition_top(memory_arena *parent, memory_arena *child, u64 size);
u64         memory_arena_save_top(memory_arena *arena);
void        memory_arena_restore_top(memory_arena *arena, u64 state);

u64         memory_arena_commit_size(memory_arena *arena);
u64         memory_arena_free_size(memory_arena *arena);
b32         memory_arena_can_accomodate(memory_arena *arena, u64 size);

#endif
