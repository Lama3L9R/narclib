//
// Created by lamadaemon on 1/9/2024.
//
// Special thanks to @7aGiven
//

#ifndef NARCHOOK_UINT96_H
#define NARCHOOK_UINT96_H

#include <cstdint>
#include "narcutils.h"

namespace narcutils::utils {

    typedef struct uint96 {
        uint64_t low;
        uint32_t high;
    } uint96_t;

    uint96_t EncryptedAPI u96_and(uint96_t a, uint96_t b) {
        uint96_t result;
        result.low = a.low & b.low;
        result.high = a.high & b. high;
        return result;
    }

    uint96_t EncryptedAPI u96_or(uint96_t a, uint96_t b) {
        uint96_t result;
        result.low = a.low | b.low;
        result.high = a.high | b. high;
        return result;
    }

    uint96_t EncryptedAPI u96_not(uint96_t a) {
        uint96_t result;
        result.low = ~a.low;
        result.high = ~a.high;
        return result;
    }

    uint96_t EncryptedAPI u96_xor(uint96_t a, uint96_t b) {
        uint96_t result;
        result.low = a.low ^ b.low;
        result.high = a.high ^ b. high;
        return result;
    }

    uint96_t EncryptedAPI u96_rtol(uint96_t a, uint32_t n) {
        uint96_t result;

        while (n > 32) {
            result.high = a.low >> 32;
            a.low = a.low << 32 | a.high;
            a.high = result.high;
            n -= 32;
        }

        result.low = a.low << n | a.high >> (32 - n);
        result.high = a.high << n | a.low >> (64 - n);

        return result;
    }


}
#endif//NARCHOOK_UINT96_H
