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
#include "ada.h"

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
                // use ada to parse the original url
                auto url = ada::parse(std::string(url_raw));
                if (!url) {
                    LOGE("Failed to parse url: %s!", url_raw);
                    return curl_vsetopt_delegation(curl_easy_handle, option, url_raw);
                }

                if (url->get_host() == "arcapi-v2.lowiro.com") {
                    url->set_host(std::string(get_custom_api_v2()));
                } else if (url->get_host() == "arcapi.lowiro.com") {
                    url->set_host(std::string(get_custom_api_legacy()));
                }

                auto new_url = url->to_string();
                LOGI("Overriding url: %s -> %s", url_raw, new_url.c_str());

                return curl_vsetopt_delegation(curl_easy_handle, option, (char*) new_url.c_str());
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

