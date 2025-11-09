#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define BUILD ".build"
#define SRC "src"
static Cmd cmd = {0};

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    cmd_append(&cmd, "clear");
    if (!cmd_run(&cmd)) return 1;

    cmd_append(&cmd, "test", "-d", ".build");
    if (!cmd_run(&cmd)) {
        cmd_append(&cmd, "mkdir", ".build");
        if (!cmd_run(&cmd)) return 1;
    }

    cmd_append(&cmd, "test", "-f", BUILD"/.gitignore");
    if (!cmd_run(&cmd)) {
        cmd_append(&cmd, "printf", "*");
        if (!cmd_run(&cmd, .stdout_path = BUILD"/.gitignore")) return 1;
    }

    nob_cc(&cmd);
    cmd_append(&cmd, "-Wall", "-Wextra", "-std=c89");
    cmd_append(&cmd, "-Wno-unused-parameter", "-Wno-unused-variable");
    // cmd_append(&cmd, "-fsanitize=address", "-fsanitize=leak", "-fsanitize=undefined", "-fsanitize=null");
    cmd_append(&cmd, "-o", BUILD"/main", SRC"/main.c");
    if (!cmd_run(&cmd)) return 1;

    cmd_append(&cmd, BUILD"/main");
    for (int i = 1; i < argc; i += 1) {
        cmd_append(&cmd, argv[i]);
    }
    if (!cmd_run(&cmd)) return 1;

    return 0;
}
