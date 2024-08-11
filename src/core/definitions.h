#ifndef SRC_CORE_DEFINITIONS_H
#define SRC_CORE_DEFINITIONS_H
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef int8_t      b8;
typedef int16_t     b16;
typedef int32_t     b32;
typedef int64_t     b64;
typedef float       r32;
typedef double      r64;
typedef const char* ccptr;
typedef char*       cptr;
typedef void*       vptr;

#define NX_BYTES(n)         ((u64)n)
#define NX_KILOBYTES(n)     (NX_BYTES(n)        * 1024)
#define NX_MEGABYTES(n)     (NX_KILOBYTES(n)    * 1024)
#define NX_GIGABYTES(n)     (NX_MEGABYTES(n)    * 1024)
#define NX_TERABYTES(n)     (NX_GIGABYTES(n)    * 1024)
#define NX_PI32             (3.14159265f)
#define NX_PI64             (3.141592653589793)
#define NX_DEGTORAD32(d)    (d/180.0f*NX_PI32)
#define NX_DEGTORAD64(d)    (d/180.0*NX_PI)

#define NX_BYTEOFFSET(ptr, n) ((u8*)ptr + n)
#define NX_BYTEOFFSET_AND_RECAST(ptr, n, type) ((type*)(u8*)ptr + n))

#if defined(NX_DEBUG_BUILD)
#   define NX_ASSERT(stm) assert((stm))
#   define NX_ENSURE_POINTER(ptr) assert(ptr != NULL)
#   define NX_NO_IMPLEMENTATION(msg) assert(!(msg))

#   if defined(NX_DEBUG_USE_PEDANTIC_ASSERT)
#       define NX_PEDANTIC_ASSERT(stm) assert((stm))
#   else
#       define NX_PEDANTIC_ASSERT(stm) 
#   endif

#else
#   define NX_ENSURE_POINTER(ptr)
#   define NX_ASSERT(stm)
#   define NX_PEDANTIC_ASSERT(stm) 
#   define NX_NO_IMPLEMENTATION(msg)
#endif

typedef struct buffer
{
    vptr ptr;
    u64 size;
} buffer;

typedef struct image
{
    vptr buffer;
    u32 width;
    u32 height;
    u32 pitch;
    u32 bits_per_pixel;
    u32 red_mask;
    u32 blue_mask;
    u32 green_mask;
    u32 alpha_mask;
} image;

#endif
