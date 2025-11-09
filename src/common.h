#ifndef GC_COMMON
#define GC_COMMON

#include <stddef.h>
#ifndef GC_INT_DEFINED
    #ifdef GC_NO_STDINT /* optional for any system that might not have stdint.h */
        typedef unsigned char      u8;
        typedef unsigned short     u16;
        typedef unsigned long int  u32;
        typedef unsigned long long u64;
        typedef   signed char      i8;
        typedef   signed short     i16;
        typedef   signed long int  i32;
        typedef   signed long long i64;
    #else /* use stdint standard types instead of c "standard" types */
        #include <stdint.h>
        typedef uint8_t     u8;
        typedef uint16_t   u16;
        typedef uint32_t   u32;
        typedef uint64_t   u64;
        typedef int8_t      i8;
        typedef int16_t    i16;
        typedef int32_t    i32;
        typedef int64_t    i64;
    #endif
    #define GC_INT_DEFINED
#endif /* GC_INT_DEFINED */

typedef struct allocator_s {
    void* (*alloc)(u64 size, void* context);
    void (*free)(void* ptr, u64 size, void* context);
    void* context;
} allocator_t;

#define make(type, size, allocator) ((type*)((allocator).alloc((u64)sizeof(type) * size, (allocator).context)))
#define release(ptr, size, allocator) ((allocator).free(ptr, size, (allocator).context))

typedef struct arena_s {
    void* buffer;
    u64 len;
    u64 cap;
} arena_t;

arena_t arena_init(void* buffer, u64 size) {
    return (arena_t){ buffer, 0, size };
}

void* arena_alloc(u64 size, void* arena) {
    arena_t* _arena = (arena_t*)arena;
    if (_arena->len + size > _arena->cap) { return NULL; }
    void* ptr = (u8*)_arena->buffer + _arena->len;
    _arena->len += size;
    return ptr;
}
void arena_clear(arena_t* arena) {
    arena->len = 0;
}
void arena_free(void* ptr, u64 size, void* arena) {
    (void)ptr;
    (void)size;
    (void)arena;
}

allocator_t arena_allocator(arena_t* arena) {
    return (allocator_t){ arena_alloc, arena_free, arena };
}

typedef struct string_s {
    char* content;
    u64 len;
} string_t;

#endif /* GC_COMMON */
