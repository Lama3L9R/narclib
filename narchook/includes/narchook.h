//
// Created by lamadaemon on 1/17/2024.
//

#ifndef NARCHOOK_NARCHOOK_H
#define NARCHOOK_NARCHOOK_H

#include "narcutils.h"

#include "lsp.h"
#include "hookapi.h"
#include "hooks/curl_hacks.h"
#include "hooks/fake_deviceid.h"

namespace narchook {
    uint8_t get_api_version();
}

#endif //NARCHOOK_NARCHOOK_H