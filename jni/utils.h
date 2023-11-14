//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_UTILS_H
#define NARCHOOK_UTILS_H

#include <string>

bool ends_with(std::string const & value, std::string const & ending);
bool contains(const uint64_t arr[], uint64_t len, uint64_t val);

template<std::size_t N, class T>
constexpr std::size_t countof(T(&)[N]) { return N; }

#endif //NARCHOOK_UTILS_H
