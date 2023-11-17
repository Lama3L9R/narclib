//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_JNI_H
#define NARCHOOK_JNI_H

#include <stdint.h>

#define ARCAEA_VERSION_UNKNOWN 0
#define ARCAEA_VERSION_PLAYSTORE 1
#define ARCAEA_VERSION_CHINA 2

#define FEAT_ARC_PLAYSTORE 0
#define FEAT_ARC_CHINA 1
#define FEAT_HOOK_SEARCHING 2

#define HOOKING_USE_OFFSET 0
#define HOOKING_USE_GENERIC_SEARCH 1
#define HOOKING_USE_CUSTOM_OFFSET 2
#define HOOKING_USE_CUSTOM_SEARCH 3

#define HOOKING_FN_VSETOPT 0

bool should_enable_hook();
bool should_enable_fake_deviceid();
bool should_override_api();
char* get_custom_api_v2();
int get_game_version();
char* get_fake_deviceid();
int get_hooking_method();

size_t get_override_offset();
uint8_t* get_override_search_seq();
size_t get_override_search_seqlen();

#endif //NARCHOOK_JNI_H
