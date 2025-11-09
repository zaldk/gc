#include <stdio.h>
#include <stdlib.h>
#include "common.h"

i32 main(i32 argc, char **argv) {
    u64 size = 128;
    void *buffer = malloc(size);
    arena_t arena = arena_init(buffer, size);
    allocator_t allocator = arena_allocator(&arena);

    u8  *x = make(u8, 8, allocator);
    u16 *y = make(u16, 8, allocator);
    u32 *z = make(u32, 8, allocator);
    u64 *w = make(u64, 8, allocator);

    u8 i = 0;
    for (i = 0; i < 8; i += 1) {
        x[i] = i+1;
        y[i] = (u16)i+1;
        z[i] = (u32)i+1;
        w[i] = (u64)i+1;
    }

    u8 j = 0;
    for (i = 0; i < arena.cap; i += 16) {
        for (j = 0; j < 16; j += 1) {
            printf("%x ", ((u8*)arena.buffer)[i+j]);
        }
        printf("\n");
    }

    free(buffer);
    return 0;
}
