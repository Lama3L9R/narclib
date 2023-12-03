//
// Created by lamadaemon on 11/13/2023.
//

#include <string>

#include "lsp.h"
#include "utils.h"
#include "log.h"
#include "hookapi.h"

#include "hooks/curl_hacks.h"
#include "hooks/fake_deviceid.h"

void on_module_loaded(const char* name, void* handle) {
    if (narchook::utils::ends_with(std::string(name), std::string(TARGET_NAME))) {
        narchook::mem::dynlib_t arcaea = narchook::mem::find_library(TARGET_NAME, handle);
        LOGI("Base is %p. Doors are now open!", arcaea.base);

        narchook::hook_all(arcaea);
    }
}

extern "C" [[gnu::visibility("default")]] [[gnu::used]]
NativeOnModuleLoaded native_init(const NativeAPIEntries *entries) {
#if NARCHOOK_BUILD_TYPE == 0
    LOGI("Welcome to narchook! Version: D%u", NARCHOOK_API_VERSION);
#else
    LOGI("Welcome to narchook! Version: R%u", NARCHOOK_API_VERSION);
#endif

    LOGI("Initializing narchook base...");
    narchook::begin(*entries);
    narchook::add_feature(narchook::hooks::curl_hacks::begin());
    narchook::add_feature(narchook::hooks::deviceid::begin());

    return on_module_loaded;
}

