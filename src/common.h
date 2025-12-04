#ifndef GC_COMMON
#define GC_COMMON


#define SHIFT(xs, xs_size) (GC_ASSERT((xs)!=NULL && (xs_size)>0), xs_size--, *(xs)++)
#define GC_ENUM(name, type) typedef type name; enum


#if defined(_WIN32) || defined(_WIN64)
    #define GC_WINDOWS
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(__POSIX__)
    #define GC_POSIX
#endif

#include <stddef.h>
#ifndef GC_INT_DEFINED
#   ifdef GC_NO_STDINT /* optional for any system that might not have stdint.h */
        typedef unsigned char        u8;
        typedef unsigned short      u16;
        typedef unsigned long int   u32;
        typedef unsigned long long  u64;
        typedef unsigned long int usize;
        typedef   signed char        i8;
        typedef   signed short      i16;
        typedef   signed long int   i32;
        typedef   signed long long  i64;
        typedef   signed long int isize;
#   else /* use stdint standard types instead of c "standard" types */
#       include <stdint.h>
        typedef uint8_t      u8;
        typedef uint16_t    u16;
        typedef uint32_t    u32;
        typedef uint64_t    u64;
        typedef uintptr_t usize;
        typedef int8_t       i8;
        typedef int16_t     i16;
        typedef int32_t     i32;
        typedef int64_t     i64;
        typedef intptr_t  isize;
#   endif
#   define GC_INT_DEFINED
#endif /* GC_INT_DEFINED */

typedef u8 bool;
#define GC_TRUE 1
#define GC_FALSE 0

#ifndef GC_NO_STDLIB
#   include <stdlib.h>
#   define GC_MALLOC malloc
#   define GC_FREE free
#endif /* GC_NO_STDLIB */

#ifndef GC_NO_STRINGLIB
#   include <string.h>
#   define GC_MEMSET memset
#   define GC_MEMCMP memcmp
#   define GC_MEMCPY memcpy
#endif /* GC_NO_STRINGLIB */

#ifndef GC_NO_STDIO
#   include <stdio.h>
#   define GC_PRINTF printf
#   define GC_LOG(log_type, fmt, ...) \
    GC_PRINTF(log_type " %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#   define GC_LOG_DEBUG(fmt, ...) GC_LOG("\e[40m\e[1;90m[DEBUG]\e[0m", fmt, ##__VA_ARGS__)
#   define GC_LOG_INFO(fmt,  ...) GC_LOG("\e[40m\e[1;34m[INFO] \e[0m", fmt, ##__VA_ARGS__)
#   define GC_LOG_WARN(fmt,  ...) GC_LOG("\e[40m\e[1;33m[WARN] \e[0m", fmt, ##__VA_ARGS__)
#   define GC_LOG_ERROR(fmt, ...) GC_LOG("\e[40m\e[1;31m[ERROR]\e[0m", fmt, ##__VA_ARGS__)
#   define GC_PRINTF_DEBUG(fmt, ...) GC_PRINTF("\e[40m\e[1;90m[DEBUG]\e[0m " fmt, ##__VA_ARGS__)
#   define GC_PRINTF_INFO(fmt,  ...) GC_PRINTF("\e[40m\e[1;34m[INFO] \e[0m " fmt, ##__VA_ARGS__)
#   define GC_PRINTF_WARN(fmt,  ...) GC_PRINTF("\e[40m\e[1;33m[WARN] \e[0m " fmt, ##__VA_ARGS__)
#   define GC_PRINTF_ERROR(fmt, ...) GC_PRINTF("\e[40m\e[1;31m[ERROR]\e[0m " fmt, ##__VA_ARGS__)
#endif /* GC_NO_STDIO */

#ifndef GC_NO_ASSERT
#   include <assert.h>
#   define GC_ASSERT assert
#endif /* GC_NO_ASSERT */

#define GC_ARENA_DEFAULT_CAPACITY (1024*1024)
typedef struct arena_s {
    void* buf;
    u64 len;
    u64 cap;
} arena_t;

/* https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator */
/* if size==0 then the capacity will be the GC_ARENA_DEFAULT_CAPACITY */
arena_t arena_alloc(u64 size);
void arena_free(arena_t* arena);

void* arena_push(arena_t* arena, u64 size);
void* arena_push_zero(arena_t* arena, u64 size);
#define ARENA_PUSH_ARRAY(arena, type, count) arena_push(arena, sizeof(type)*(count))
#define ARENA_PUSH_ARRAY_ZERO(arena, type, count) arena_push_zero(arena, sizeof(type)*(count))
#define ARENA_PUSH_STRUCT(arena, type) arena_push(arena, sizeof(type))
#define ARENA_PUSH_STRUCT_ZERO(arena, type) arena_push_zero(arena, sizeof(type))

void arena_pop(arena_t* arena, u64 size);
void arena_clear(arena_t* arena);

u64 arena_get_len(arena_t* arena);
void arena_set_len(arena_t* arena, u64 len);

/* prints the contents of the arena in xxd style */
#define ARENA_INSPECT(arena) arena_inspect(arena, __LINE__, __FILE__)


#ifndef GC_COMMON_IMPLEMENTATION
#define GC_COMMON_IMPLEMENTATION
/* {{{ */

arena_t arena_alloc(u64 size) {
    u64 cap = size > 0 ? size : GC_ARENA_DEFAULT_CAPACITY;
    u8* buffer = (u8*)GC_MALLOC(cap);
    GC_MEMSET(buffer, 0, cap);
    return (arena_t){ buffer, 0, cap };
}
void arena_free(arena_t* arena) {
    GC_FREE(arena->buf);
}

void* arena_push(arena_t* arena, u64 size) {
    if (arena->len + size > arena->cap) { return NULL; }
    void* ptr = (u8*)arena->buf + arena->len;
    arena->len += size;
    return ptr;
}
void* arena_push_zero(arena_t* arena, u64 size) {
    void* ptr = arena_push(arena, size);
    if (ptr == NULL) { return NULL; }
    GC_MEMSET(ptr, 0, size);
    return ptr;
}

void arena_pop(arena_t* arena, u64 size) {
    if (size > arena->len) {
        arena->len = 0;
    } else {
        arena->len -= size;
    }
}
void arena_clear(arena_t* arena) {
    arena->len = 0;
}

u64 arena_get_len(arena_t* arena) {
    return arena->len;
}
void arena_set_len(arena_t* arena, u64 len) {
    arena->len = len;
}

void arena_inspect(arena_t* arena, u64 line, const char* file) {
    u8* buf = (u8*)arena->buf;
    u64 pos_last_byte = arena->cap-1;

    if (pos_last_byte == 0 || buf == NULL) goto empty_arena;
    while (pos_last_byte > 0 && buf[pos_last_byte] == 0) { pos_last_byte -= 1; }
    if (pos_last_byte == 0) goto empty_arena;
    goto non_empty_arena;

empty_arena:
    GC_LOG_DEBUG("<ARENA EMPTY>");
    return;

non_empty_arena:
    GC_LOG_DEBUG( "<ARENA HEX DUMP> (buf=%p cap=%ld len=%ld) [%s:%ld]", arena->buf, arena->cap, arena->len, file, line);
    u64 i, j;
    for (i = 0; i <= pos_last_byte; i += 16) {
        GC_PRINTF("%08lX:", i);
        for (j = 0; j < 16; j += 1) {
            if ((j&1) == 0) GC_PRINTF(" ");
            GC_PRINTF("%02x", buf[i+j]);
        }
        GC_PRINTF("  ");
        for (j = 0; j < 16; j += 1) {
            if (' ' <= buf[i+j] && buf[i+j] <= '~') {
                GC_PRINTF("%c", buf[i+j]);
            } else {
                GC_PRINTF(".");
            }
        }
        GC_PRINTF("\n");
    }
}

/* }}} */
#endif /* GC_COMMON_IMPLEMENTATION */

#endif /* GC_COMMON */
