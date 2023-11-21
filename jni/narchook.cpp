//
// Created by lamadaemon on 11/13/2023.
//

#include <string>
#include <jni.h>

#include "narchook.h"
#include "lsp.h"
#include "offset.h"
#include "japi.h"
#include "utils.h"
#include "log.h"

static HookFunType hook = nullptr;
static UnhookFunType unhook = nullptr;

uint64_t *(*Arc_CURL_vsetopt)(void* curl_easy_handle, int32_t option, va_list param);
void *(*Arc_Game_setDeviceId)(JNIEnv* env, jclass clazz, jstring id);

uint64_t* Arcaea_CURL_vsetopt(void* curl_easy_handle, int32_t option, va_list param) {
    return Arc_CURL_vsetopt(curl_easy_handle, option, param);
}

uint64_t *Arc_CURL_vsetopt_callback(void* curl_easy_handle, int32_t option, va_list param) {
    if (!should_enable_hook()) {
        return Arc_CURL_vsetopt(curl_easy_handle, option, param);
    }


    if (contains(option_deny_list, countof(option_deny_list), option)) {
        LOGD("Invoke <CALL-REJECTED> CURL_vsetopt: %d", option);

        return CURL_SUCCESS;
    }

    LOGD("Invoke CURL_vsetopt: %d", option);

    switch (option) {
        case CURLOPT_SSL_VERIFYPEER:
        {
            return curl_vsetopt_delegation(curl_easy_handle, option, 0);
        }

        case CURLOPT_URL:
        {
            char* url_raw = va_arg(param, char *);
            if (url_raw != nullptr && should_override_api()) {
                std::string url = std::string(url_raw);
                // Find out the protocol
                size_t protocol_index = url.find("://");
                std::string protocol = url.substr(0, protocol_index);

                // Find out the host and path
                size_t host_end_index = url.find('/', protocol_index + 3);
                std::string host = url.substr(protocol_index + 3, host_end_index - protocol_index - 3);
                std::string path = url.substr(host_end_index);

                std::string new_url = protocol + "://" + std::string(get_custom_api_v2()) + path;

                // According to https://curl.se/libcurl/c/CURLOPT_URL.html, we should free the string
                char* cstr_new_url = strdup(new_url.c_str());
                LOGI("API Overriding: %s -> %s", url_raw, cstr_new_url);

                va_list args = build_args(0, cstr_new_url);
                uint64_t* rtn = curl_vsetopt_delegation(curl_easy_handle, option, cstr_new_url);

                free(cstr_new_url);

                return rtn;
            } else {
                return curl_vsetopt_delegation(curl_easy_handle, option, url_raw);;
            }
        }

        case CURLOPT_SSL_VERIFYHOST: {
            return curl_vsetopt_delegation(curl_easy_handle, option, 0);
        }
    }

    return Arc_CURL_vsetopt(curl_easy_handle, option, param);
}

void *Arc_Game_setDeviceId_callback(JNIEnv* env, jclass clazz, jstring id) {
    if (!should_enable_hook() && should_enable_fake_deviceid()) {
        return Arc_Game_setDeviceId(env, clazz, env->NewStringUTF(get_fake_deviceid()));
    }
    LOGI("Setting device id to %s", env->GetStringUTFChars(id, JNI_FALSE));

    return Arc_Game_setDeviceId(env, clazz, id);
}

void on_module_loaded(const char* name, void* handle) {
    if (ends_with(std::string(name), std::string(TARGET_NAME))) {
        void* base = get_base();

        LOGI("Base is %p. Doors are now open!", base);

        if (is_target_supported()) {
            hook(get_Arc_CURL_vsetopt(), (void*) Arc_CURL_vsetopt_callback, (void**) &Arc_CURL_vsetopt);
            hook(get_Arc_Game_setDeviceId(handle), (void*) Arc_Game_setDeviceId_callback, (void**) &Arc_Game_setDeviceId);

        } else {
            LOGE("Unsupported version! Refusing to hook (version: %d)", get_game_version());
        }
    }
}

extern "C" [[gnu::visibility("default")]] [[gnu::used]]
NativeOnModuleLoaded native_init(const NativeAPIEntries *entries) {
    hook = entries->hook_func;
    unhook = entries->unhook_func;

#if NARCHOOK_BUILD_TYPE == 0
    LOGI("Welcome to narchook! Version: D%u", NARCHOOK_API_VERSION);
#else
    LOGI("Welcome to narchook! Version: R%u", NARCHOOK_API_VERSION);
#endif

    return on_module_loaded;
}

