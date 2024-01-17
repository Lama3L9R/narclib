//
// Created by lamadaemon on 11/30/2023.
//

#ifndef NARCHOOK_HOOKAPI_H
#define NARCHOOK_HOOKAPI_H

#include "narchook.h"
#include "lsp.h"

#define HOOKING_USE_OFFSET         0
#define HOOKING_USE_PATTERN_SEARCH 1
#define HOOKING_USE_EXPORT_NAME    2

namespace narchook {
    typedef struct hook_env        hook_env_t;
    typedef struct hooking_feature hooking_feature_t;
    typedef struct narchook        narchook_t;
    typedef struct hooking_param   hooking_param_t;
    typedef union hooking_target   hooking_target_t;

    union hooking_target {
        size_t offset;

        struct {
            uint8_t* pattern;
            size_t   len;
        } search;

        struct {
            char* name;
        } named;
    };

    struct hooking_param {
        bool             is_supported;
        uint8_t          method;
        hooking_target_t target;
    };

    struct hook_env {
        narchook_t*   narchook;
        void*         base;
        mem::dynlib_t lib;
        HookFunType   hook_fn;
        UnhookFunType unhook_fn;
    };

    struct hooking_feature {
        uint32_t        feature;
        bool            is_enabled;
        uint8_t         hooking_method; /** Default hooking method */
        hooking_param_t params[3];

        void* handle_hook;

        void** original_fn;
    };

    struct narchook {
        bool is_enabled;

        hooking_feature_t* features;
        size_t             features_len;
    };

    extern hooking_param_t OFFSET_NOT_SUPPORTED;

    extern hooking_param_t PATTERN_NOT_SUPPORTED;

    extern hooking_param_t NAME_NOT_SUPPORTED;

    void begin(const NativeAPIEntries* entries);
    void add_feature(hooking_feature_t feature);
    void vadd_feature(hooking_feature_t features[], size_t hook_len);
    void hook_all(mem::dynlib_t lib);

    hook_env_t* get_env();

    bool is_feature_enabled(uint32_t feature);
    void set_feature_enabled(uint32_t feature, bool enabled);

    void end();
}// namespace narchook


#endif//NARCHOOK_HOOKAPI_H
