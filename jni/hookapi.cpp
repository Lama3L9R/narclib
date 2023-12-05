//
// Created by lamadaemon on 11/30/2023.
//

#include <cstdlib>
#include "hookapi.h"
#include "mem.h"
#include "log.h"

namespace narchook {
    hooking_param_t OFFSET_NOT_SUPPORTED = {
            .is_supported = false,
            .method = HOOKING_USE_OFFSET,
            .target = {
                    .offset = 0
            }
    };

    hooking_param_t PATTERN_NOT_SUPPORTED = {
            .is_supported = false,
            .method = HOOKING_USE_PATTERN_SEARCH,
            .target = {
                    .offset = 0
            }
    };

    hooking_param_t NAME_NOT_SUPPORTED = {
            .is_supported = false,
            .method = HOOKING_USE_EXPORT_NAME,
            .target = {
                    .offset = 0
            }
    };

    narchook_t* archooks;
    hook_env_t* env;

    void begin(const NativeAPIEntries* entries) {
        archooks = (narchook_t*) malloc(sizeof(narchook_t));
        env = (hook_env_t*) malloc(sizeof(hook_env_t));

        env->narchook = archooks;
        env->hook_fn = entries->hook_func;
        env->unhook_fn = entries->unhook_func;

        archooks->features = nullptr;
        archooks->features_len = 0;
        archooks->is_enabled = true;
    }

    void add_feature(hooking_feature_t feature) {
        if (archooks->features == nullptr) {
            archooks->features = (hooking_feature_t*) mem::alloc(sizeof(hooking_feature_t));
            archooks->features[0] = feature;
            archooks->features_len = 1;
        } else {
            archooks->features_len ++;
            archooks->features = (hooking_feature_t*) realloc(archooks->features, sizeof(hooking_feature_t) * archooks->features_len);
            archooks->features[archooks->features_len - 1] = feature;
        }
    }

    hook_env_t* get_env() {
        return env;
    }

    bool is_feature_enabled(uint32_t feature) {
        for (int i = 0; i < archooks->features_len; i++) {
            hooking_feature_t feat = archooks->features[i];
            if (feat.feature == feature && feat.is_enabled) {
                return true;
            }
        }

        return false;
    }

    void set_feature_enabled(uint32_t feature, bool enabled) {
        for (int i = 0; i < archooks->features_len; i++) {
            hooking_feature_t* feat = &archooks->features[i];
            if (feat->feature == feature) {
                feat->is_enabled = enabled;
                return;
            }
        }

    }

    void hook_all(mem::dynlib_t lib) {
        env->lib = lib;
        env->base = env->lib.base;

        for (size_t i = 0; i < archooks->features_len; i ++) {
            hooking_feature_t* feat = &archooks->features[i];
            if (feat->is_enabled) {
                hooking_param* available_params[3];
                for (int j = 0; j < 3; j++) {
                    available_params[j] = &feat->params[j];
                    if (available_params[j]->is_supported && available_params[j]->method == feat->hooking_method) {
                        hooking_param* tmp = available_params[0];
                        available_params[0] = available_params[j];
                        available_params[j] = tmp;
                    }
                }

                void* hook_fn;

                for (int j = 0; j < 3; j++) {
                    hooking_param* param = available_params[j];
                    if (available_params[j]->is_supported) {
                        hooking_target_t target = param->target;

                        switch (param->method) {
                            case HOOKING_USE_OFFSET:
                                hook_fn = mem::from_offset(&env->lib, target.offset);
                                break;
                            case HOOKING_USE_PATTERN_SEARCH:
                                hook_fn = mem::find_pattern(&env->lib, target.search.pattern, target.search.len);
                                break;
                            case HOOKING_USE_EXPORT_NAME:
                                hook_fn = mem::from_export_name(&env->lib, target.named.name);
                                break;
                        }

                        if (hook_fn != nullptr) {
                            break;
                        }
                    }
                }

                if (hook_fn == nullptr) {
                    LOGE("Failed to hook feature %d! Existed hooking strategies failed to locate target address", feat->feature);
                    continue;
                }

                env->hook_fn(hook_fn, feat->handle_hook, feat->original_fn);
            }
        }
    }

    void end() {
        // Currently there is no support for dynamic overriding of hooking parameters
        // So all of them should be allocated in read-only area. No need to free them.
        // In future, after implementing dynamic overriding, this code should be uncommented and completed.
//        for (int i = 0; i < archooks->features_len; i++) {
//            hooking_feature_t* feat = &archooks->features[i];
//
//            for (int j = 0; j < 3; j ++) {
//                hooking_param_t param = feat->params[j];
//                // TODO: free allocated memory
//            }
//
//
//        }

        free(archooks->features);
        free(archooks);
        free(env);
    }
}