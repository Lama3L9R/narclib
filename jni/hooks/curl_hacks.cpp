//
// Created by lamadaemon on 12/2/2023.
//

#include "hooks/curl_hacks.h"
#include "ada.h"
#include "hooks/data.h"
#include "log.h"
#include "utils.h"
#include <jni.h>

uint64_t* (*Arc_CURL_vsetopt)(void* curl_easy_handle, int32_t option, va_list param);

static const uint8_t pattern[] = CURL_VSETOPT_PATTERN;

static narchook::utils::dynarr_uint32_t option_deny_list    = narchook::utils::dynarr_uint32_init();
static char*                            custom_api_v2       = nullptr;
static char*                            custom_api_legacy   = nullptr;
static bool                             disable_ssl_pinning = false;
static char*                            dump_cert_path      = nullptr;
static bool                             ended               = false;

typedef struct curl_blob {
    void*        data;
    size_t       len;
    unsigned int flags;
} curl_blob_t;

uint64_t* Arc_CURL_vsetopt_callback(void* curl_easy_handle, int32_t option, va_list param) {
    if (ended) {
        return Arc_CURL_vsetopt(curl_easy_handle, option, param);
    }

    if (!narchook::is_feature_enabled(FEAT_CURLHACKS)) {
        return Arc_CURL_vsetopt(curl_easy_handle, option, param);
    }

    if (narchook::utils::dynarr_uint32_contains(&option_deny_list, option)) {
        LOGD("Invoke <CALL-REJECTED> CURL_vsetopt: %d", option);

        return CURL_SUCCESS;
    }

    LOGD("Invoke CURL_vsetopt: %d", option);

    switch (option) {
        case CURLOPT_SSL_VERIFYPEER: {
            LOGI("cURL SSL verification disabled!");
            return Arcaea::CURL::easy_setopt(curl_easy_handle, option, 0);
        }

        case CURLOPT_URL: {
            char* url_raw = va_arg(param, char*);
            if (url_raw != nullptr && (custom_api_v2 != nullptr)) {
                auto url = ada::parse(std::string(url_raw));
                if (!url) {
                    LOGE("Failed to parse url: %s!", url_raw);
                    return Arcaea::CURL::easy_setopt(curl_easy_handle, option, url_raw);
                }

                if (url->get_host() == "arcapi-v2.lowiro.com") {
                    url->set_host(std::string(custom_api_v2));
                } else if (url->get_host() == "arcapi.lowiro.com") {
                    if (custom_api_legacy == nullptr) {
                        LOGW("Legacy API is not set! Falling back to arcapi-v2.");
                        url->set_host(std::string(custom_api_v2));
                    } else {
                        url->set_host(std::string(custom_api_legacy));
                    }
                }

                char* new_url = narchook::utils::stringview_to_cstr(url->get_href());
                LOGI("Overriding url: %s -> %s", url_raw, new_url);

                uint64_t* result = Arcaea::CURL::easy_setopt(curl_easy_handle, option, new_url);
                free(new_url);

                return result;
            } else {
                return Arcaea::CURL::easy_setopt(curl_easy_handle, option, url_raw);
            }
        }

        case CURLOPT_SSL_VERIFYHOST: {
            return Arcaea::CURL::easy_setopt(curl_easy_handle, option, 0);
        }

        case CURLOPT_PINNEDPUBLICKEY: {
            if (disable_ssl_pinning) {
                LOGI("cURL SSL pinning disabled!");
                return CURL_SUCCESS;
            }
        }

        case CURLOPT_SSLCERT_BLOB: {
            if (dump_cert_path != nullptr) {
                curl_blob_t* cert = va_arg(param, curl_blob_t*);

                if (cert == nullptr) {
                    LOGE("Failed to dump cert: cert is null!");
                    return Arcaea::CURL::easy_setopt(curl_easy_handle, option, cert);
                }

                FILE* f = fopen(dump_cert_path, "wb");

                if (f == nullptr) {
                    uint32_t err = ferror(f);
                    LOGE("Failed to dump cert: %s (%d)", strerror(err), err);
                    goto DUMP_CERT_FINALIZE;
                }

                if (!fwrite(cert->data, cert->len, 1, f)) {
                    LOGE("Failed to dump cert: failed to write file (%d)", ferror(f));
                    goto DUMP_CERT_CLOSE;
                }

            DUMP_CERT_CLOSE:
                fclose(f);
                clearerr(f);
            DUMP_CERT_FINALIZE:
                return Arcaea::CURL::easy_setopt(curl_easy_handle, option, cert);
            }
        }

        case CURLOPT_SSLKEYPASSWD: {
            va_list param_copy;
            va_copy(param_copy, param);
            char* passwd = va_arg(param_copy, char*);
            LOGI("cURL using client certificate with password: %s", passwd);
            va_end(param_copy);
        }
    }

    uint64_t* result = Arc_CURL_vsetopt(curl_easy_handle, option, param);
    LOGD("Http request done with code %zu", (size_t) result);
    return result;
}

namespace narchook::hooks::curl_hacks {
    void set_param(uint32_t key, ...) {
        va_list param;
        va_start(param, key);
        switch (key) {
            case PARAM_CURLHACKS_NOSSLPINNING: {
                disable_ssl_pinning = va_arg(param, uint32_t) != 0;

                break;
            }
            case PARAM_CURLHACKS_OVERRIDEAPIV2: {
                char* url = va_arg(param, char*);
                if (custom_api_v2 != nullptr) {
                    free(custom_api_v2);
                }

                if (url == nullptr) {
                    custom_api_v2 = nullptr;
                } else {
                    custom_api_v2 = strdup(url);
                }
                break;
            }
            case PARAM_CURLHACKS_OVERRIDEAPI: {
                char* url = va_arg(param, char*);
                if (custom_api_legacy != nullptr) {
                    free(custom_api_legacy);
                }

                if (url == nullptr) {
                    custom_api_legacy = nullptr;
                } else {
                    custom_api_legacy = strdup(url);
                }
                break;
            }
            case PARAM_CURLHACKS_SAVECERT: {
                char* path = va_arg(param, char*);
                if (dump_cert_path != nullptr) {
                    free(dump_cert_path);
                }
                dump_cert_path = strdup(path);
                break;
            }
            case PARAM_CURLHACKS_ADDDENYLISTE: {
                uint32_t option = va_arg(param, uint32_t);
                utils::dynarr_uint32_append(&option_deny_list, option);
                break;
            }
            case PARAM_CURLHACKS_REMOVEDENYLISTE: {
                uint32_t option = va_arg(param, uint32_t);
                utils::dynarr_uint32_remove(&option_deny_list, option);
                break;
            }
        }

        va_end(param);
    }

    hooking_feature_t begin() {
        // clang-format off
        return hooking_feature_t{
            .feature        = FEAT_CURLHACKS,
            .is_enabled     = false,
            .hooking_method = HOOKING_USE_PATTERN_SEARCH,
            .params         = {
            hooking_param {
                    .is_supported = true,
                    .method       = HOOKING_USE_PATTERN_SEARCH,
                    .target       = hooking_target_t{
                        .search = {
                            .pattern = (uint8_t*) &pattern,
                            .len     = sizeof pattern
                        }
                    }
                },OFFSET_NOT_SUPPORTED, NAME_NOT_SUPPORTED },
            .handle_hook    = (void*) Arc_CURL_vsetopt_callback,
            .original_fn    = (void**) &Arc_CURL_vsetopt
        };
        // clang-format on
    }

    void end() {
        if (custom_api_v2 != nullptr) {
            free(custom_api_v2);
        }

        if (custom_api_legacy != nullptr) {
            free(custom_api_legacy);
        }

        if (dump_cert_path != nullptr) {
            free(dump_cert_path);
        }

        utils::dynarr_uint32_end(&option_deny_list);

        ended = true;
    }
}// namespace narchook::hooks::curl_hacks

namespace Arcaea::CURL {
    uint64_t* easy_setopt(void* curl_easy_handle, int32_t option, ...) {
        va_list param;
        va_start(param, option);

        auto result = Arc_CURL_vsetopt(curl_easy_handle, option, param);

        va_end(param);

        return result;
    }

}// namespace Arcaea::CURL

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_setCustomAPI(JNIEnv* env, jclass clazz, jstring url_) {
    const char* newhost = env->GetStringUTFChars(url_, JNI_FALSE);
    char*       host_c  = strdup(newhost);
    narchook::hooks::curl_hacks::set_param(PARAM_CURLHACKS_OVERRIDEAPI, host_c);
    free(host_c);
    env->ReleaseStringUTFChars(url_, newhost);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_setCustomAPIV2(JNIEnv* env, jclass clazz, jstring url_) {
    const char* newhost = env->GetStringUTFChars(url_, JNI_FALSE);
    char*       host_c  = strdup(newhost);
    narchook::hooks::curl_hacks::set_param(PARAM_CURLHACKS_OVERRIDEAPIV2, host_c);
    free(host_c);
    env->ReleaseStringUTFChars(url_, newhost);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_toggleDisableSSLPinning(JNIEnv* env, jclass clazz, jboolean disable) {
    narchook::hooks::curl_hacks::set_param(PARAM_CURLHACKS_NOSSLPINNING, disable);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_addDenyList(JNIEnv* env, jclass clazz, jint option) {
    narchook::hooks::curl_hacks::set_param(PARAM_CURLHACKS_ADDDENYLISTE, option);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_removeDenyList(JNIEnv* env, jclass clazz, jint option) {
    narchook::hooks::curl_hacks::set_param(PARAM_CURLHACKS_REMOVEDENYLISTE, option);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_dumpCert(JNIEnv* env, jclass clazz, jstring path_) {
    const char* path = env->GetStringUTFChars(path_, JNI_FALSE);
    narchook::hooks::curl_hacks::set_param(PARAM_CURLHACKS_SAVECERT, strdup(path));
    env->ReleaseStringUTFChars(path_, path);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_CURLHacks_toggleFeature(JNIEnv* env, jclass clazz, jboolean enabled) {
    narchook::set_feature_enabled(FEAT_CURLHACKS, enabled);
}
