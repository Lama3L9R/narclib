//
// Created by lamadaemon on 11/13/2023.
//
#include "utils.h"
#include "narchook.h"

bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool contains(const uint64_t arr[], uint64_t len, uint64_t val) {
    for (int i = 0; i < len; i++) {
        if (arr[i] == val) {
            return true;
        }
    }
    return false;
}

va_list build_args(int dummy, ...) {
    va_list args;
    va_start(args, dummy);
    return args;
}

uint64_t* curl_vsetopt_delegation(void* curl_easy_handle, int32_t option, ...) {
    va_list args;
    uint64_t* rtn;
    va_start(args, option);
    rtn = Arcaea_CURL_vsetopt(curl_easy_handle, option, args);
    va_end(args);

    return rtn;
}