//
// Created by lamadaemon on 1/9/2024.
//
// Special thanks to @7aGiven
//

#ifndef NARCHOOK_UIN96_H
#define NARCHOOK_UIN96_H

#include <cstdint>

namespace narchook::utils {

    typedef struct uint96 {   
        uint64_t low;
        uint32_t high;
    } uint96_t;

    uint96_t u96_and(uint96_t* a, uint96_t* b);
    uint96_t u96_or(uint96_t* a, uint96_t* b);
    uint96_t u96_not(uint96_t* a, uint96_t* b);
    uint96_t u96_xor(uint96_t* a, uint96_t* b);
    uint96_t u96_rtol(uint96_t* b, uint32_t n);


}
#endif//NARCHOOK_UINT96_H
