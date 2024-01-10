//
// Created by lamadaemon on 1/9/2024.
//
// Special thanks to @7aGiven
//

#ifndef NARCHOOK_ARCHASH_H
#define NARCHOOK_ARCHASH_H

#include <cstdint>
#include "hookapi.h"
#include "archash/uint96.h"

namespace narchook::archash {

    void sponge(utils::uint96_t* raw, utils::uint96_t** result);
    void hash(const char* raw, size_t len, char** result);

}

#endif//NARCHOOK_ARCHASH_H
