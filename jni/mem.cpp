//
// Created by lamadaemon on 12/2/2023.
//

#include "mem.h"
#include "hookapi.h"
#include "log.h"
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace narchook::mem {
    typedef struct dynmodule {
        uintptr_t* base;
        size_t     size;
    } dynmodule_t;

    EncryptedAPI bool find_module_base(const std::string& module_name, dynmodule_t* module) {
        std::ifstream maps("/proc/self/maps");
        std::string   line;
        while (std::getline(maps, line)) {
            if (line.find(module_name) != std::string::npos) {
                std::istringstream iss(line);
                std::string        start_address;
                std::string        end_address;
                std::getline(iss, start_address, '-');
                std::getline(iss, end_address, ' ');
                module->base  = (uintptr_t*) std::stoull(start_address, nullptr, 16);
                uintptr_t end = std::stoull(end_address, nullptr, 16);
                module->size  = end - (uintptr_t) module->base;

                LOGI("Found module %s at %p with size %zu", module_name.c_str(), module->base, module->size);
                return true;
            }
        }
        return false;
    }

    EncryptedAPI dynlib_t find_library(const char* name) {
        dynlib_t lib;
        lib.libname = (char*) name;
        lib.handle  = nullptr;
        dynmodule_t module;

        if (!find_module_base(name, &module)) {
            LOGE("Failed to find module %s", name);
            return lib;
        }

        lib.base = module.base;
        lib.size = module.size;

        return lib;
    }

    EncryptedAPI dynlib_t find_library(const char* name, void* handle) {
        dynlib_t lib = find_library(name);
        lib.handle   = handle;
        return lib;
    }

    EncryptedAPI void* find_pattern(dynlib_t* lib, uint8_t* pattern, size_t len) {
        return memmem(lib->base, lib->size, pattern, len);
    }

    EncryptedAPI void* from_offset(dynlib_t* lib, uintptr_t offset) {
        return (void*) ((uintptr_t) lib->base + offset);
    }

    EncryptedAPI void* from_export_name(dynlib_t* lib, const char* name) {
        if (lib->handle == nullptr) {
            LOGE("Cannot find export %s from library %s without handle!", name, lib->libname);
            return nullptr;
        }

        return dlsym(lib->handle, name);
    }

    EncryptedAPI void* alloc(size_t size) {
        void* data = malloc(size);
        if (data == nullptr) {
            LOGE("Failed to allocate %zu bytes of memory", size);
        }

        memset(data, 0, size);
        return data;
    }
}// namespace narchook::mem
