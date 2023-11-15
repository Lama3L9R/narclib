//
// Created by lamadaemon on 11/14/2023.
//

#include <dlfcn.h>
#include "offset.h"
#include "japi.h"

void* from_offset(void* base, size_t offset) {
    return (void*) ((size_t) base + offset);
}

void* get_Arc_CURL_vsetopt(void* base) {
    if (get_game_version() == ARCAEA_VERSION_UNKNOWN) {
        return nullptr;
    }

    if (get_game_version() == ARCAEA_VERSION_PLAYSTORE) {
        return from_offset(base, OFFSET_P_Arc_CURL_vsetopt);
    } else {
        return from_offset(base, OFFSET_C_Arc_CURL_vsetopt);
    }
}

void* get_Arc_Game_setDeviceId(void* base) {
#ifdef SET_DEVICEID_USE_OFFSET
    if (get_game_version() == ARCAEA_VERSION_UNKNOWN) {
        return nullptr;
    }

    if (get_game_version() == ARCAEA_VERSION_PLAYSTORE) {
        return from_offset(base, JNI_SET_DEVICEID_OFFSET_PLAYSTORE);
    } else {
        return from_offset(base, JNI_SET_DEVICEID_OFFSET_CHINA);
    }
#endif

    return dlsym(base, JNI_SET_DEVICEID_NAME);
}

bool is_target_supported() {
    if (get_game_version() == ARCAEA_VERSION_UNKNOWN) {
        return false;
    }

    if (get_game_version() == ARCAEA_VERSION_PLAYSTORE) {
#ifdef NO_PLAYSTORE_SUPPORT
        return false
#else
        return true;
#endif
    } else if (get_game_version() == ARCAEA_VERSION_CHINA) {
#ifdef NO_CHINA_SUPPORT
        return false
#else
        return true;
#endif
    }
}