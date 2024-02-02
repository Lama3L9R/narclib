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

#define LOG_PROVIDED

#endif

#ifndef LOG_PROVIDED
#include <cstdarg>

#ifndef LOG_PROVIDED

#include <cstdarg>
#include <cstdio>


#define LOGE narcutils::log::error
#define LOGW narcutils::log::warn
#define LOGI narcutils::log::info
#define LOGD narcutils::log::debug

namespace narcutils::log {
    void EncryptedAPI vlog(const char tag[], const char format[], va_list args) {
#ifdef NARCHOOK_BUILD_DEBUG
        size_t final_format_len = strlen(tag) + 3 + strlen(format) + 1 + 1;
        char* final_format_buff = (char*) malloc(final_format_len);

        snprintf(final_format_buff, final_format_len, "[%s] %s\n", tag, format);
        vprintf(final_format_buff, args);

        free(final_format_buff);
#endif
    }

    void EncryptedAPI debug(const char format[], ...) {
        va_list args;
        va_start(args, format);
        vlog("narchash::debug", format, args);
        va_end(args);
    }

    void EncryptedAPI info(const char format[], ...) {
        va_list args;
        va_start(args, format);
        vlog("narchash::info", format, args);
        va_end(args);
    }

    void EncryptedAPI warn(const char format[], ...) {
        va_list args;
        va_start(args, format);
        vlog("narchash::warn", format, args);
        va_end(args);
    }

    void EncryptedAPI error(const char format[], ...) {
        va_list args;
        va_start(args, format);
        vlog("narchash::error", format, args);
        va_end(args);
    }
}
#endif



#endif

#endif//NARCHOOK_LOG_H
