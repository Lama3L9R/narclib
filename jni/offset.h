//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_OFFSET_H
#define NARCHOOK_OFFSET_H

#define CURLOPT_SSLCERTTYPE 10086
#define CURLOPT_SSLKEYPASSWD 10026
#define CURLOPT_SSLCERT_BLOB 40291
#define CURLOPT_PINNEDPUBLICKEY 10230
#define CURLOPT_SSL_VERIFYPEER 64
#define CURLOPT_SSL_VERIFYHOST 81
#define CURLOPT_URL 10002

#define CURL_SUCCESS 0uLL
#define CURL_ERROR 1uLL

#define TARGET_NAME "libcocos2dcpp.so"
const uint64_t option_deny_list[] = {CURLOPT_PINNEDPUBLICKEY};

#ifdef OVERRIDE_OFFSET_VSETOPT_PLAYSTORE
#define OFFSET_Arc_CURL_vsetopt OVERRIDE_OFFSET_VSETOPT_PLAYSTORE
#pragma message "Overriding offset for PlayStore version"
#else
#define OFFSET_P_Arc_CURL_vsetopt 0x79C234
#endif

#ifdef OVERRIDE_OFFSET_VSETOPT_CHINA
#define OFFSET_Arc_CURL_vsetopt OVERRIDE_OFFSET_VSETOPT_CHINA
#pragma message "Overriding offset for China version"
#else
#define OFFSET_C_Arc_CURL_vsetopt 0xED0064
#endif

#ifdef OVERRIDE_JNI_SET_DEVICEID_NAME
#define JNI_SET_DEVICEID_NAME OVERRIDE_JNI_SET_DEVICEID_NAME
#pragma message "Overriding JNI setDeviceId name"
#else
#define JNI_SET_DEVICEID_NAME "Java_low_moe_AppActivity_setDeviceId"
#endif

#ifdef JNI_SET_DEVICEID_OFFSET_PLAYSTORE
#ifndef SET_DEVICEID_USE_OFFSET
#define SET_DEVICEID_USE_OFFSET
#endif
#define JNI_SET_DEVICEID_OFFSET JNI_SET_DEVICEID_OFFSET_PLAYSTORE
#endif

#ifdef JNI_SET_DEVICEID_OFFSET_CHINA
#ifndef SET_DEVICEID_USE_OFFSET
#define SET_DEVICEID_USE_OFFSET
#endif
#define JNI_SET_DEVICEID_OFFSET JNI_SET_DEVICEID_OFFSET_CHINA
#endif

#ifdef SET_DEVICEID_USE_OFFSET
#warning "Using offset instead of symbol name for setDeviceId"
#endif

#ifdef ENABLE_GENERIC_HOOKING
#define VSETOPT_PATTERN {0xFF, 0x43, 0x01, 0xD1, 0xF7, 0x0B, 0x00, 0xF9, 0xF6, 0x57, 0x02, 0xA9, 0xF4, 0x4F, 0x03, 0xA9, 0xFD, 0x7B, 0x04, 0xA9, 0xFD, 0x03, 0x01, 0x91, 0x08, 0x02, 0x82, 0x52, 0x09, 0xE2, 0x84, 0x52, 0xF4, 0x03, 0x02, 0xAA, 0xF3, 0x03, 0x00, 0xAA, 0x16, 0x00, 0x08, 0x8B, 0x3F, 0x00, 0x09, 0x6B, 0x80, 0x00, 0x80, 0x52, 0xCD, 0x05, 0x00, 0x54, 0xE8, 0xCA, 0x89, 0x52, 0x3F, 0x00, 0x08, 0x6B, 0xAC, 0x09, 0x00, 0x54, 0x08, 0xE2, 0x84, 0x12, 0x28, 0x00, 0x08, 0x0B, 0x1F, 0xA5, 0x04, 0x71, 0x28, 0x1E, 0x00, 0x54};
#endif

void* get_base();

void* get_Arc_CURL_vsetopt();
#ifdef SET_DEVICEID_USE_OFFSET
void* get_Arc_Game_setDeviceId();
#else
void* get_Arc_Game_setDeviceId(void* base);
#endif

bool is_target_supported();

#endif //NARCHOOK_OFFSET_H
