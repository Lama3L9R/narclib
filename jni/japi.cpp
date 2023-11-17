//
// Created by lamadaemon on 11/13/2023.
//
#include "japi.h"
#include "log.h"
#include <jni.h>
#include <cstring>
#include <cstdlib>

bool enable_hook = false;
bool enable_fake_deviceid = false;
char* newId = nullptr;
char* url = nullptr;
int hooking_method = HOOKING_USE_GENERIC_SEARCH;
size_t override_offset = 0;
uint8_t* override_search = nullptr;
size_t override_search_len = 0;

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


extern "C" JNIEXPORT [[deprecated("Will be replaced by isFeatureSupported")]]
jboolean JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_isVersionSupported(JNIEnv* env, jclass clazz, jint version) {
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

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_overrideHookingMethod(JNIEnv* env, jclass clazz, jint method) {
    hooking_method = method;
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_overrideOffset(JNIEnv* env, jclass clazz, jint fn, jint offset) {
    if (HOOKING_FN_VSETOPT == fn) {
        override_offset = offset;
    }
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_overridePattern(JNIEnv* env, jclass clazz, jbyteArray pattern) {
    jboolean copy = JNI_FALSE;
    char* buff = (char*) env->GetByteArrayElements(pattern, &copy);
    override_search_len = env->GetArrayLength(pattern);;
    override_search = (uint8_t*) malloc(override_search_len);
    memcpy(override_search, buff, override_search_len);
    env->ReleaseByteArrayElements(pattern, (jbyte*) buff, JNI_ABORT);
}

extern "C" JNIEXPORT jboolean JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_isFeatureSupported(JNIEnv* env, jclass clazz, jlong feature) {
    switch(feature) {
        case FEAT_ARC_PLAYSTORE:
#ifdef NO_PLAYSTORE_SUPPORT
            return JNI_FALSE;
#else
            return JNI_TRUE;
#endif
        case FEAT_ARC_CHINA:
#ifdef NO_CHINA_SUPPORT
            return JNI_FALSE;
#else
            return JNI_TRUE;
#endif
        case FEAT_HOOK_SEARCHING:
#ifdef ENABLE_GENERIC_HOOKING
            return JNI_TRUE;
#else
            return JNI_FALSE;
#endif
    }
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

int get_hooking_method() {
    return hooking_method;
}

size_t get_override_offset() {
    return override_offset;
}

uint8_t* get_override_search_seq() {
    return override_search;
}

size_t get_override_search_seqlen() {
    return override_search_len;
}