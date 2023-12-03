//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_UTILS_H
#define NARCHOOK_UTILS_H

#include <string>

namespace narchook::utils {
    typedef struct dynarr_uint32 {
        size_t len;
        uint32_t* data;
    } dynarr_uint32_t;

    dynarr_uint32 dynarr_uint32_init();
    void dynarr_uint32_append(dynarr_uint32_t* arr, uint32_t val);
    void dynarr_uint32_remove(dynarr_uint32_t* arr, uint32_t val);
    bool dynarr_uint32_contains(dynarr_uint32_t* arr, uint32_t val);
    void dynarr_uint32_end(dynarr_uint32_t* arr);

    bool ends_with(std::string const & value, std::string const & ending);
}

#endif //NARCHOOK_UTILS_H
