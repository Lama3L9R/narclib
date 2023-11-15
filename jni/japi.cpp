//
// Created by lamadaemon on 11/13/2023.
//
#include "japi.h"
#include "log.h"
#include <jni.h>
#include <cstring>

bool enable_hook = false;
bool enable_fake_deviceid = false;
char* newId = nullptr;
char* url = nullptr;

int gameversion = ARCAEA_VERSION_UNKNOWN;

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_setUrl(JNIEnv *env, jclass clazz, jstring url_) {
    url = strdup(env->GetStringUTFChars(url_, 0));
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_enable(JNIEnv *env, jclass clazz, jboolean status) {
    enable_hook = status;
}

extern "C" JNIEXPORT jstring JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_getTargetVersion(JNIEnv* env, jclass clazz) {
    return env->NewStringUTF(TARGET_VERSION);
}

extern "C" JNIEXPORT jboolean JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_isVersionSupported(JNIEnv* env, jclass clazz, jint version) {
    switch(version) {
        case ARCAEA_VERSION_PLAYSTORE:
#ifdef NO_PLAYSTORE_SUPPORT
            return JNI_FALSE;
#endif
            break;
        case ARCAEA_VERSION_CHINA:
#ifdef NO_CHINA_SUPPORT
            return JNI_FALSE;
#endif
            break;
        default:
            return JNI_FALSE;
    }
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_setHookTarget(JNIEnv* env, jclass clazz, jint version) {
    LOGI("Setting hook target to %d", version);
    gameversion = version;
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_enableFakeDeviceID(JNIEnv* env, jclass clazz, jboolean status) {
    enable_fake_deviceid = status;
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_setFakeDeviceID(JNIEnv* env, jclass clazz, jstring id_) {
    newId = strdup(env->GetStringUTFChars(id_, 0));
}

bool should_enable_hook() {
    return enable_hook;
}

bool should_override_api() {
    return url != nullptr;
}

char* get_custom_api_v2() {
    return url;
}

int get_game_version() {
    return gameversion;
}

bool should_enable_fake_deviceid() {
    return enable_fake_deviceid;
}

char* get_fake_deviceid() {
    return newId;
}