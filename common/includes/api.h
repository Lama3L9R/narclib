//
// Created by lamadaemon on 1/16/2024.
//

#ifndef NARCHOOK_API_H
#define NARCHOOK_API_H

#ifdef NARCHOOK_PLATFORM_CLANG

#define HookAPI [[gnu::visibility("default")]]
#define EncryptedAPI inline __attribute__((always_inline))

#endif

#ifdef NARCHOOK_PLATFORM_MSVC

#define HookAPI __declspec(dllexport)
#define EncryptedAPI __forceinline

#endif

#ifndef HookAPI
#define HookAPI
#endif

#ifndef EncryptedAPI
#define EncryptedAPI
#endif

#endif//NARCHOOK_API_H
