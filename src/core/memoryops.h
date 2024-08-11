#ifndef SRC_CORE_MEMORYOPS_H
#define SRC_CORE_MEMORYOPS_H
#include <core/definitions.h>

void memory_copy_simple(vptr dest, const vptr source, u64 size);
void memory_copy_ext(vptr dest, const vptr source, u64 size);
void memory_set_zero_simple(vptr dest, u64 size);
void memory_set_zero_ext(vptr dest, u64 size);

#endif
