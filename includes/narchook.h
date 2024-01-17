//
// Created by lamadaemon on 1/17/2024.
//

#ifndef NARCHOOK_NARCHOOK_H
#define NARCHOOK_NARCHOOK_H

#include "api.h"
#include "log.h"
#include "utils/mem.h"
#include "utils/utils.h"

#if NARCHOOK_BUILD_VARIANT == 0
#include "xposed/hookapi.h"
#include "xposed/hooks/curl_hacks.h"
#include "xposed/hooks/fake_deviceid.h"
#include "xposed/lsp.h"
#elif NARCHOOK_BUILD_VARIANT == 1
#include "utils/base64.h"
#include "archash/archash.h"
#include "archash/uint96.h"
#endif

#include <cstdint>

namespace narchook {
    uint8_t get_api_version();
}

#endif//NARCHOOK_NARCHOOK_H
