//
// Created by lamadaemon on 11/13/2023.
//
#include "utils.h"

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