#include "common.h"

bool is_dir(char* path);

#ifndef GC_FS_IMPLEMENTATION
#define GC_FS_IMPLEMENTATION

#ifdef GC_POSIX
#include <sys/stat.h>
bool is_dir(char* path) {
    struct stat s;
    if (stat(path, &s) == 0 && S_ISDIR(s.st_mode)) {
        return GC_TRUE;
    }
    return GC_FALSE;
}
#endif

#endif /* GC_FS_IMPLEMENTATION */
