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
