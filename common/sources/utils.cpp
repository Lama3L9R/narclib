//
// Created by lamadaemon on 12/2/2023.
//

#include "narcutils.h"
#include "utils.h"
#include <cstdlib>
#include <string>

namespace narcutils::utils {
    dynarr_uint32 dynarr_uint32_init() {
        // clang-format off
        return dynarr_uint32_t { 0, nullptr };
        // clang-format on
    }

    void dynarr_uint32_append(dynarr_uint32_t* arr, uint32_t val) {
        if (arr->len == 0) {
            arr->data    = (uint32_t*) malloc(sizeof(uint32_t));
            arr->data[0] = val;
        } else {
            arr->data             = (uint32_t*) realloc(arr->data, sizeof(uint32_t) * (arr->len + 1));
            arr->data[arr->len++] = val;
        }
    }

    void dynarr_uint32_remove(dynarr_uint32_t* arr, uint32_t val) {
        if (arr->len == 0) {
            return;
        }

        uint32_t* new_arr = (uint32_t*) malloc(sizeof(uint32_t) * (arr->len - 1));
        for (int i = 0; i < arr->len; i++) {
            if (arr->data[i] != val) {
                new_arr[i] = arr->data[i];
            }
        }

        arr->data = new_arr;
    }

    bool dynarr_uint32_contains(dynarr_uint32_t* arr, uint32_t val) {
        if (arr->len == 0) {
            return false;
        }

        for (int i = 0; i < arr->len; i++) {
            if (arr->data[i] == val) {
                return true;
            }
        }

        return false;
    }

    void dynarr_uint32_end(dynarr_uint32_t* arr) {
        if (arr->len == 0) {
            return;
        }

        free(arr->data);
        arr->len = 0;
    }

    bool ends_with(std::string const& value, std::string const& ending) {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    char* stringview_to_cstr(std::string_view sv) {
        std::string str = { sv.begin(), sv.end() };

        return strdup(str.c_str());
    }
}