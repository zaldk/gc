#include "common.h"
#include "fs.h"

GC_ENUM(gc_cli_actor_t, u8) {
    GC_ACTOR_NONE = 0,
    GC_ACTOR_PUSH,
    GC_ACTOR_PULL,
    GC_ACTOR_SAVE,
    GC_ACTOR_CONFIG,
};

GC_ENUM(gc_cli_config_t, u8) {
    GC_CONFIG_LOCAL = 0,
    GC_CONFIG_GLOBAL,
};

typedef struct {
    gc_cli_actor_t actor;
    gc_cli_config_t config_type;
    char* save_src;
    char* pull_src;
    char* push_dst;
} cli_options_t;

bool parse_cli_options(i32 argc, char** argv, cli_options_t* options);
bool is_valid_push_dst(char* dst);
bool is_valid_pull_src(char* src);
bool is_valid_save_src(char* src);


#ifndef GC_CLI_IMPLEMENTATION
#define GC_CLI_IMPLEMENTATION

bool parse_cli_options(i32 argc, char** argv, cli_options_t* options) {
    #define EQ(str1, str2) (0 == GC_MEMCMP(str1, str2, sizeof(str2)))
    SHIFT(argv, argc); // the name of the exe is ingored

    if (argc == 0) {
        GC_LOG_ERROR("No command provided");
        return GC_FALSE;
    }
    char* command = SHIFT(argv, argc);

    options->actor = GC_ACTOR_NONE;
    if (EQ(command, "push")) {
        options->actor = GC_ACTOR_PUSH;
        if (argc == 0) {
            options->push_dst = ".";
            goto ret;
        }
        char* dst = SHIFT(argv, argc);
        if (!is_valid_push_dst(dst)) {
            GC_LOG_ERROR("Invalid push destination: '%s'", dst);
            return GC_FALSE;
        }
        options->push_dst = dst;
        goto ret;
    }
    if (EQ(command, "pull")) {
        options->actor = GC_ACTOR_PULL;
        if (argc == 0) {
            GC_LOG_ERROR("Command `pull` expects a source.");
            return GC_FALSE;
        }
        char* src = SHIFT(argv, argc);
        if (!is_valid_pull_src(src)) {
            GC_LOG_ERROR("Invalid pull source: '%s'", src);
            return GC_FALSE;
        }
        options->pull_src = src;
        goto ret;
    }
    if (EQ(command, "save")) {
        options->actor = GC_ACTOR_SAVE;
        if (argc == 0) {
            GC_LOG_ERROR("Command `save` expects a source.");
            return GC_FALSE;
        }
        char* src = SHIFT(argv, argc);
        if (!is_valid_save_src(src)) {
            GC_LOG_ERROR("Invalid save source: '%s'", src);
            return GC_FALSE;
        }
        options->save_src = src;
        goto ret;
    }
    if (EQ(command, "config")) {
        if (argc == 0) {
            GC_LOG_ERROR("Command `config` expects a type.");
            return GC_FALSE;
        }
        options->actor = GC_ACTOR_CONFIG;
        char* type = SHIFT(argv, argc);
        goto ret;
    }

ret:
    if (argc > 0) {
        GC_PRINTF_ERROR("Unused arguments:");
        while (argc > 0) {
            GC_PRINTF(" %s", SHIFT(argv, argc));
        }
        GC_PRINTF("\n");
        return GC_FALSE;
    }
    if (options->actor == GC_ACTOR_NONE) {
        GC_LOG_ERROR("Invalid command: '%s'", command);
        return GC_FALSE;
    }
    return GC_TRUE;

    #undef EQ
}

bool is_valid_push_dst(char* dst) {
    return is_dir(dst);
}
bool is_valid_pull_src(char* src) {
    return is_dir(src);
}
bool is_valid_save_src(char* src) {
    return is_dir(src);
}


#endif /* GC_CLI_IMPLEMENTATION */
