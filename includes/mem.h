//
// Created by lamadaemon on 12/2/2023.
//

#ifndef NARCHOOK_MEM_H
#define NARCHOOK_MEM_H

#include <string>

namespace narchook::mem {
    typedef struct dynlib dynlib_t;

    struct dynlib {
        char*  libname;
        void*  handle;
        void*  base;
        size_t size;
    };

    dynlib_t find_library(const char* name);
    dynlib_t find_library(const char* name, void* handle);
    void*    find_pattern(dynlib_t* lib, uint8_t* pattern, size_t len);
    void*    from_offset(dynlib_t* lib, uintptr_t offset);
    void*    from_export_name(dynlib_t* lib, const char* name);
    void*    alloc(size_t size);

}// namespace narchook::mem

#endif//NARCHOOK_MEM_H
