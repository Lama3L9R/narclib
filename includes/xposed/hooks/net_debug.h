//
// Created by lamadaemon on 1/4/2024.
//

#ifndef NARCHOOK_NET_DEBUG_H
#define NARCHOOK_NET_DEBUG_H

#include "narchook.h"

#define FEAT_NETDEBUG_EASYPERFORM 2
#define FEAT_NETDEBUG_SSLGETERROR 3

namespace narchook::hooks::net_debug {

    void begin();

    void end();
}// namespace narchook::hooks::net_debug

namespace Arcaea::SSL {
    uint64_t SSL_get_error(void* sslctx, int ret);
}

namespace Arcaea::CURL {
    uint64_t easy_perform(void* curl_easy_handle);
}


#endif//NARCHOOK_NET_DEBUG_H
