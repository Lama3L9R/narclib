//
// Created by lamadaemon on 11/13/2023.
//

#include "narchook.h"
#include <jni.h>

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_NArcHook_toggleFeature(JNIEnv* env, jclass clazz, jint feature, jboolean enabled) {
    narchook::set_feature_enabled(feature, enabled);
}

extern "C" JNIEXPORT jint JNICALL Java_icu_lama_narchook_NArcHook_getVersion(JNIEnv* env, jclass clazz) {
    return NARCHOOK_API_VERSION;
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_NArcHook_end(JNIEnv* env, jclass clazz) {
    narchook::hooks::deviceid::end();
    narchook::hooks::curl_hacks::end();
    narchook::end();
}
