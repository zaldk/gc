#include "common.h"

i32 main(i32 argc, char **argv) {
    arena_t arena = arena_alloc(256);

    u64 len = 128;
    u8* x = ARENA_PUSH_ARRAY_ZERO(&arena, u8, len);
    for (u64 i = 0; i < len; i++) x[i] = i;

    ARENA_INSPECT(&arena);
    arena_free(&arena);
    return 0;
}
