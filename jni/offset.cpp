//
// Created by lamadaemon on 11/14/2023.
//

#include <dlfcn.h>
#include <string>
#include "offset.h"
#include "japi.h"
#include "log.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>

void* base = nullptr;

void* from_offset(void* base, uintptr_t offset) {
    return (void*) ((uintptr_t) base + offset);
}

uintptr_t get_module_base_address(const std::string& module_name) {
    std::ifstream maps("/proc/self/maps");
    std::string line;
    while (std::getline(maps, line)) {
        if (line.find(module_name) != std::string::npos) {
            std::istringstream iss(line);
            std::string address;
            std::getline(iss, address, '-');
            return std::stoull(address, 0, 16);
        }
    }
    return 0;
}

void* from_byte_sequence(void* begin, char* search_seq, size_t len, size_t max_searches) {
    if (max_searches <= 0) {
        return nullptr;
    }

    if (memcmp(begin, search_seq, len) < 0) {
        return from_byte_sequence((void*)((size_t) begin + 1), search_seq, len, --max_searches);
    }

    return begin;
}


void* get_Arc_CURL_vsetopt() {
    if (get_game_version() == ARCAEA_VERSION_UNKNOWN) {
        return nullptr;
    }

    switch (get_hooking_method()) {
        case HOOKING_USE_OFFSET:
            if (get_game_version() == ARCAEA_VERSION_PLAYSTORE) {
                LOGI("Using offset %p", from_offset(get_base(), OFFSET_P_Arc_CURL_vsetopt));
                return from_offset(get_base(), OFFSET_P_Arc_CURL_vsetopt);
            } else {
                return from_offset(get_base(), OFFSET_C_Arc_CURL_vsetopt);
            }
        case HOOKING_USE_CUSTOM_OFFSET:
            return from_offset(get_base(), get_override_offset());
        case HOOKING_USE_GENERIC_SEARCH:
#ifdef ENABLE_GENERIC_HOOKING
            uint8_t* pattern = get_override_search_seq();
            size_t len = get_override_search_seqlen();
            if (pattern == nullptr) {
                uint8_t pattern_default[] = VSETOPT_PATTERN;
                pattern = (uint8_t*) &pattern_default;
                len = countof(pattern_default);
            }

            void* fn = from_byte_sequence(get_base(), (char*) &pattern, len, 0x900000);
            if (fn != nullptr) {
                LOGW("Using generic hooking for Arc_CURL_vsetopt: %p", fn);
                return fn;
            } else {
                LOGW("Generic hooking failed, using offset instead!");
            }
#else
            return nullptr;
#endif
    }
    

}


#ifdef SET_DEVICEID_USE_OFFSET
void* get_Arc_Game_setDeviceId() {
    if (get_game_version() == ARCAEA_VERSION_UNKNOWN) {
        return nullptr;
    }

    if (get_game_version() == ARCAEA_VERSION_PLAYSTORE) {
        return from_offset(get_base(), JNI_SET_DEVICEID_OFFSET_PLAYSTORE);
    } else {
        return from_offset(get_base(), JNI_SET_DEVICEID_OFFSET_CHINA);
    }
    return nullptr;
}
#else
void* get_Arc_Game_setDeviceId(void* handle) {
    return dlsym(handle, JNI_SET_DEVICEID_NAME);
}
#endif

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
#ifdef USE_GENERIC_HOOKING
    return true;
#endif
    return false;
}


void* get_base() {
    if (base == nullptr) {
        base = (void*) get_module_base_address("libcocos2dcpp.so");
    }

    return base;
}