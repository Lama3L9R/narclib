//
// Created by lamadaemon on 11/13/2023.
//

#include <string>
#include <jni.h>

#include "lsp.h"
#include "offset.h"
#include "japi.h"
#include "utils.h"

static HookFunType hook = nullptr;
static UnhookFunType unhook = nullptr;

uint64_t *(*Arc_CURL_vsetopt)(void* curl_easy_handle, int32_t option, void* param);
void *(*Arc_Game_setDeviceId)(JNIEnv* env, jclass clazz, jstring id);

uint64_t *Arc_CURL_vsetopt_callback(void* curl_easy_handle, int32_t option, void* param) {
    if (!should_enable_hook()) {
        return Arc_CURL_vsetopt(curl_easy_handle, option, param);
    }

    if (contains(option_deny_list, countof(option_deny_list), option)) {
        return CURL_SUCCESS;
    }

    switch (option) {
        case CURLOPT_SSL_VERIFYPEER:
            return Arc_CURL_vsetopt(curl_easy_handle, CURLOPT_SSL_VERIFYPEER, nullptr);
        case CURLOPT_URL:
            if (param != nullptr && should_override_api()) {
                std::string url = std::string((char*) param);
                // Find out the protocol
                size_t protocol_index = url.find("://");
                std::string protocol = url.substr(0, protocol_index);

                // Find out the host and path
                size_t host_end_index = url.find("/", protocol_index + 3);
                std::string host = url.substr(protocol_index + 3, host_end_index - protocol_index - 3);
                std::string path = url.substr(host_end_index);

                std::string new_url = protocol + "://" + std::string(get_custom_api_v2()) + path;

                // According to https://curl.se/libcurl/c/CURLOPT_URL.html, we should free the string
                char* cstr_new_url = strdup(new_url.c_str());
                uint64_t* rtn = Arc_CURL_vsetopt(curl_easy_handle, CURLOPT_URL, cstr_new_url);
                free(cstr_new_url);

                return rtn;
            }
            break;
    }

    return Arc_CURL_vsetopt(curl_easy_handle, option, param);
}

void *Arc_Game_setDeviceId_callback(JNIEnv* env, jclass clazz, jstring id) {
    if (!should_enable_hook() && should_enable_fake_deviceid()) {
        return Arc_Game_setDeviceId(env, clazz, env->NewStringUTF(get_fake_deviceid()));
    }
    return Arc_Game_setDeviceId(env, clazz, id);
}


extern "C" [[gnu::visibility("default")]] [[gnu::used]]
NativeOnModuleLoaded native_init(const NativeAPIEntries *entries) {
    hook = entries->hook_func;
    unhook = entries->unhook_func;
}

extern "C" [[gnu::visibility("default")]] [[gnu::used]]
NativeOnModuleLoaded on_module_loaded(const char* name, void* handle) {
    if (ends_with(std::string(name), std::string(libname)) && is_target_supported()) {
        hook(get_Arc_CURL_vsetopt(handle), (void*) Arc_CURL_vsetopt_callback, (void**) &Arc_CURL_vsetopt);
        hook(get_Arc_Game_setDeviceId(handle), (void*) Arc_Game_setDeviceId_callback, (void**) &Arc_Game_setDeviceId);
    }
}

