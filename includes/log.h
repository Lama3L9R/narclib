//
// Created by lamadaemon on 11/14/2023.
//

#ifndef NARCHOOK_LOG_H
#define NARCHOOK_LOG_H

#include <android/log.h>

#define TAG "narchook"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

#endif//NARCHOOK_LOG_H
