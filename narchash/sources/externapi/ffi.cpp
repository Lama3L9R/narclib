//
// Created by lamadaemon on 1/30/2024.
//

#include "narchash.h"

extern "C" HookAPI uint8_t FFI_narchash_generate_challenge(char* path, char* body, uint64_t ts, uint8_t salt[32], char** result) {
    return narchash::generate_challenge(path, body, ts, salt, result);
}

extern "C" HookAPI uint8_t FFI_narchash_verify_challenge(char* challenge, char* path, char* body, uint64_t ts, uint8_t salt[32]) {
    return narchash::verify_challenge(challenge, path, body, ts, salt);
}

extern "C" HookAPI uint32_t FFI_narchash_get_api_version() {
    return narchash::get_api_version();
}