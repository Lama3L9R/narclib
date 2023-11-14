//
// Created by lamadaemon on 11/13/2023.
//
#include "jniconf.h"
#include <jni.h>
#include <cstring>

char* url = nullptr;
bool enable_hook = false;

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_setUrl(JNIEnv *env, jclass clazz, jstring url_) {
    url = strdup(env->GetStringUTFChars(url_, 0));
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_ukbeggar_hooks_NArcHook_enable(JNIEnv *env, jclass clazz, jboolean status) {
    enable_hook = status;
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