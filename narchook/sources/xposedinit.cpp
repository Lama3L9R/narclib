//
// Created by lamadaemon on 11/13/2023.
//

#include <string>

#include "narchook.h"
#include "narcutils.h"
#include "hooks/data.h"

#ifdef NARCHOOK_BUILD_DEBUG
#include "hooks/net_debug.h"
#endif

void on_module_loaded(const char* name, void* handle) {
    if (narcutils::utils::ends_with(std::string(name), std::string(TARGET_NAME))) {
        narcutils::mem::dynlib_t arcaea = narcutils::mem::find_library(TARGET_NAME, handle);
        LOGI("Base is %p. Doors are now open!", arcaea.base);

        narchook::hook_all(arcaea);
    }
}

extern "C" [[gnu::visibility("default")]] [[gnu::used]] NativeOnModuleLoaded native_init(const NativeAPIEntries* entries) {
#ifdef NARCHOOK_BUILD_DEBUG
    LOGI("Welcome to narchook! Version: D%u", NARCHOOK_API_VERSION);
#else
    LOGI("Welcome to narchook! Version: R%u", NARCHOOK_API_VERSION);
#endif

    LOGI("Initializing narchook base...");
    narchook::begin(entries);
    LOGI("Initializing features...");
    narchook::hooks::curl_hacks::begin();
    narchook::hooks::deviceid::begin();
#ifdef NARCHOOK_BUILD_DEBUG
#warning Network debug enabled
    narchook::hooks::net_debug::begin();
#endif
    LOGI("Narchook base initialized! Ready to give guy a f**k!");

    return on_module_loaded;
}
