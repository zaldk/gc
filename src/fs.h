/* an abstraction layer over filesystem-specific operations */

#include "common.h"

typedef struct file_s {
    FILE fd;
} file_t;

