//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_JNI_H
#define NARCHOOK_JNI_H

#define ARCAEA_VERSION_UNKNOWN 0
#define ARCAEA_VERSION_PLAYSTORE 1
#define ARCAEA_VERSION_CHINA 2

bool should_enable_hook();
bool should_enable_fake_deviceid();
bool should_override_api();
char* get_custom_api_v2();
int get_game_version();
char* get_fake_deviceid();

#endif //NARCHOOK_JNI_H
