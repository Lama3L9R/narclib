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

    void sponge(uint96_t* raw, uint96_t** result);
    void hash(char* raw, size_t len, char** result);

}

#endif//NARCHOOK_ARCHASH_H
