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
#define CURLOPT_URL 10002

#define CURL_SUCCESS 0

#define TARGET_NAME "libcocos2dcpp.so"
const uint64_t option_deny_list[] = {CURLOPT_SSLCERTTYPE, CURLOPT_SSLKEYPASSWD, CURLOPT_SSLCERT_BLOB, CURLOPT_PINNEDPUBLICKEY};

#ifdef OVERRIDE_OFFSET_VSETOPT_PLAYSTORE
#define OFFSET_Arc_CURL_vsetopt OVERRIDE_OFFSET_VSETOPT_PLAYSTORE
#pragma message "Overriding offset for PlayStore version"
#else
#define OFFSET_P_Arc_CURL_vsetopt 0xA3C364
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

void* get_base();

void* get_Arc_CURL_vsetopt();
#ifdef SET_DEVICEID_USE_OFFSET
void* get_Arc_Game_setDeviceId();
#else
void* get_Arc_Game_setDeviceId(void* base);
#endif

bool is_target_supported();

#endif //NARCHOOK_OFFSET_H
