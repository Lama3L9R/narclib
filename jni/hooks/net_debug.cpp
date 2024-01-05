//
// Created by lamadaemon on 1/4/2024.
//

#include "hooks/net_debug.h"
#include "hooks/data.h"
#include "log.h"

static const uint8_t easy_perform_pattern[] = CURL_EASY_PERFORM_PATTERN;

uint64_t (*Arc_CURL_easy_perform)(void* curl_easy_handle);

uint64_t Arc_CURL_easy_perform_callback(void* curl_easy_handle) {
    uint64_t result = Arcaea::CURL::easy_perform(curl_easy_handle);
    LOGI("NetDebug easy_perform return %zu", result);

    return result;
}

uint64_t (*Arc_SSL_get_error)(void* sslctx, int ret);

uint64_t Arc_SSL_get_error_callback(void* sslctx, int ret) {
    uint64_t result = Arcaea::SSL::SSL_get_error(sslctx, ret);
    LOGI("NetDebug SSL_get_error return %zu", result);

    return result;
}

namespace narchook::hooks::net_debug {
    void begin() {
        // clang-format off
        add_feature(hooking_feature_t {
            .feature        = FEAT_NETDEBUG_EASYPERFORM,
            .is_enabled     = true,
            .hooking_method = HOOKING_USE_PATTERN_SEARCH,
            .params         = {
                hooking_param {
                    .is_supported = true,
                    .method       = HOOKING_USE_PATTERN_SEARCH,
                    .target       = hooking_target_t {
                        .search = {
                            .pattern = (uint8_t*) &easy_perform_pattern,
                            .len     = sizeof easy_perform_pattern
                        }
                    }
                }, OFFSET_NOT_SUPPORTED, NAME_NOT_SUPPORTED },
            .handle_hook    = (void*) Arc_CURL_easy_perform_callback,
            .original_fn    = (void**) &Arc_CURL_easy_perform
        });

        add_feature(hooking_feature_t {
            .feature        = FEAT_NETDEBUG_SSLGETERROR,
            .is_enabled     = false,
            .hooking_method = HOOKING_USE_EXPORT_NAME,
            .params         = {PATTERN_NOT_SUPPORTED, OFFSET_NOT_SUPPORTED, hooking_param {
                .is_supported = true,
                .method       = HOOKING_USE_EXPORT_NAME,
                .target       = hooking_target_t {
                    .named = {
                        .name = strdup(OPENSSL_GET_ERROR)
                    }
                }
            }},
            .handle_hook    = (void*) Arc_SSL_get_error_callback,
            .original_fn    = (void**) &Arc_SSL_get_error
        });
        // clang-format on
    }


    void end() { }
}

namespace Arcaea::SSL {
    uint64_t SSL_get_error(void* sslctx, int ret) {
        return Arc_SSL_get_error(sslctx, ret);
    }
}

namespace Arcaea::CURL {
    uint64_t easy_perform(void* curl_easy_handle) {
        return Arc_CURL_easy_perform(curl_easy_handle);
    }
}