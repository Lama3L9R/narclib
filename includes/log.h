//
// Created by lamadaemon on 11/14/2023.
//

#ifndef NARCHOOK_LOG_H
#define NARCHOOK_LOG_H

#ifdef NARCHOOK_PLATFORM_ANDROID

#define TAG "narchook"
#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

#endif

#ifndef LOGE

#define LOGE(...)
#define LOGW(...)
#define LOGI(...)
#define LOGD(...)

#endif

#endif//NARCHOOK_LOG_H
