//
// Created by lamadaemon on 1/16/2024.
//

#ifndef NARCHOOK_API_H
#define NARCHOOK_API_H

#ifdef NARCHOOK_PLATFORM_CLANG

#define HookAPI [[gnu::visibility("default")]] [[gnu::used]]
#define EncryptedAPI [[clang::always_inline]] [[gnu::used]] inline

#endif

#ifdef NARCHOOK_PLATFORM_MSVC

#define HookAPI __declspec(dllexport)
#define EncryptedAPI __forceinline

#endif

#endif//NARCHOOK_API_H
