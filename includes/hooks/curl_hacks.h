//
// Created by lamadaemon on 12/2/2023.
//

#ifndef NARCHOOK_CURL_HACKS_H
#define NARCHOOK_CURL_HACKS_H

#include <cstdint>
#include <cstdarg>

#include "hookapi.h"

#define FEAT_CURLHACKS 0

#define PARAM_CURLHACKS_NOSSLPINNING 0
#define PARAM_CURLHACKS_OVERRIDEAPIV2 1
#define PARAM_CURLHACKS_OVERRIDEAPI 2
#define PARAM_CURLHACKS_SAVECERT 3
#define PARAM_CURLHACKS_ADDDENYLISTE 4
#define PARAM_CURLHACKS_REMOVEDENYLISTE 5

#define CURLHACKS_VSETOPT_PATTERN "\xFF\x43\x01\xD1\xF7\x0B\x00\xF9\xF6\x57\x02\xA9\xF4\x4F\x03\xA9\xFD\x7B\x04\xA9\xFD\x03\x01\x91\x08\x02\x82\x52\x09\xE2\x84\x52\xF4\x03\x02\xAA\xF3\x03\x00\xAA\x16\x00\x08\x8B\x3F\x00\x09\x6B\x80\x00\x80\x52\xCD\x05\x00\x54\xE8\xCA\x89\x52\x3F\x00\x08\x6B\xAC\x09\x00\x54\x08\xE2\x84\x12\x28\x00\x08\x0B\x1F\xA5\x04\x71\x28\x1E\x00\x54"

#define CURLOPT_SSLCERTTYPE 10086
#define CURLOPT_SSLKEYPASSWD 10026
#define CURLOPT_SSLCERT_BLOB 40291
#define CURLOPT_PINNEDPUBLICKEY 10230
#define CURLOPT_SSL_VERIFYPEER 64
#define CURLOPT_SSL_VERIFYHOST 81
#define CURLOPT_URL 10002

#define CURL_SUCCESS 0uLL
#define CURL_ERROR 1uLL


namespace narchook::hooks::curl_hacks {

    void set_param(uint32_t param, ...);

    hooking_feature_t begin();

    void end();
}

/**
 * @brief Namespace Arcaea provides a set of delegated functions for easy access to the game's internals functions.
 */
namespace Arcaea::CURL {
    uint64_t* easy_setopt(void* curl_easy_handle, int32_t option, ...);
}

#endif //NARCHOOK_CURL_HACKS_H
