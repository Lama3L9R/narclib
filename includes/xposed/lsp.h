//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_LSP_H
#define NARCHOOK_LSP_H
#include <cstdint>

typedef int (*HookFunType)(void *func, void *replace, void **backup);

typedef int (*UnhookFunType)(void *func);

typedef void (*NativeOnModuleLoaded)(const char *name, void *handle);

typedef struct {
    uint32_t version;
    HookFunType hook_func;
    UnhookFunType unhook_func;
} NativeAPIEntries;

typedef NativeOnModuleLoaded (*NativeInit)(const NativeAPIEntries *entries);
#endif //NARCHOOK_LSP_H
