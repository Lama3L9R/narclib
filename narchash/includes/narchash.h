//
// Created by lamadaemon on 1/9/2024.
//
// Special thanks to @7aGiven
//

#ifndef NARCHOOK_NARCHASH_H
#define NARCHOOK_NARCHASH_H

#include "narcutils.h"
#include <cstdint>
#include <cstring>
#include <ctime>


#include "uint96.h"

using namespace narcutils::utils;

#define HASH_SIZE 48

#define VERIFY_NO_ERROR 0
#define VERIFY_OUTDATED 1
#define VERIFY_ILLEGAL_FORMAT 2
#define VERIFY_ILLEGAL_HASH 3

#define GENERATE_SUCCESS 0
#define GENERATE_FAILED_GETTIME 1

#define TIMESTAMP_GETSYSTEM 0

typedef uint8_t arc_salt_t[32];

namespace narchash {

    static unsigned char hcst[] = {0x04,0x07,0x01,0x0D,0x15,0x25,0x44,0x06,0x03,0x09,0x1D,0x35,0x64,0x47,0x00,0x0F,0x11,0x2D,0x54,0x26,0x42,0x0A,0x1B,0x39,0x7C,0x77,0x61,0x4D,0x14,0x27,0x40,0x0E,0x13,0x29,0x5C,0x36,0x62,0x4B,0x18,0x3F,0x70,0x6F,0x51,0x2C,0x56,0x22,0x4A,0x1A,0x3B,0x78,0x7F,0x71,0x6D,0x55,0x24,0x46,0x02,0x0B,0x19,0x3D,0x74,0x67,0x41,0x0C,0x17,0x21,0x4C,0x16,0x23,0x48,0x1E,0x33,0x68,0x5F,0x30,0x6E,0x53,0x28,0x5E,0x32,0x6A,0x5B,0x38,0x7E,0x73,0x69,0x5D,0x34,0x66,0x43,0x08,0x1F,0x31,0x6C,0x57,0x20,0x4E,0x12,0x2B,0x58,0x3E,0x72,0x6B,0x59};

    constexpr size_t challenge_size() {
        return sizeof(uint64_t) + HASH_SIZE + HASH_SIZE;
    }

    EncryptedAPI uint64_t get_ts_now() {
        struct timespec ts { };
        timespec_get(&ts, TIME_UTC);

        return ts.tv_sec * 1000 + ts.tv_nsec / 1e6;
    }

    EncryptedAPI void sponge(uint96_t* raw, uint96_t** result) {
        size_t len = 6 * sizeof (uint96_t);
        uint96_t buff[6] = { 0 };
        memcpy(buff, raw, len);

        for (size_t i = 0; i < 104; i++) {
            buff[0].low ^= hcst[i];
            buff[3] = u96_xor(u96_not(buff[5]), buff[0]);
            buff[2] = u96_xor(u96_or(buff[1], buff[4]), buff[3]);
            buff[4] = u96_xor(u96_and(buff[1], u96_not(buff[0])), buff[4]);
            buff[0] = u96_and(buff[3], buff[4]);
            buff[3] = u96_xor(buff[1], buff[5]);
            buff[0] = u96_xor(buff[0], buff[3]);
            buff[3] = u96_xor(u96_and(buff[2], buff[3]), buff[4]);
            buff[4] = u96_xor(buff[4], buff[5]);

            buff[5] = u96_rtol(buff[4], 55);
            buff[4] = u96_rtol(buff[2], 8);
            buff[1] = u96_rtol(buff[3], 1);
        }

        if (*result == nullptr) {
            (*result) = (uint96_t*) narcutils::mem::alloc(len);
        }

        memcpy(*result, buff, len);
    }

    EncryptedAPI void hash(const unsigned char* raw, size_t len, char** result) {
        uint96_t* buff = (uint96_t*) narcutils::mem::alloc(6 * sizeof(uint96_t));

        size_t n = 0;
        uint64_t block = 0;

        while (n + 6 <= len) {
            block = *(uint64_t*) (raw + n) & 0xFFFFFFFFFFFF;
            buff[0].low ^= block;

            uint96_t* new_buff = nullptr;
            sponge(buff, &new_buff);
            memcpy(buff, new_buff, sizeof(uint96_t) * 6);
            free(new_buff);

            n += 6;
        }

        len -= n;
        raw += n;
        block = 0;
        for (n = 0; n < len; n++) {
            block |= (uint64_t) raw[n] << 8 * n;
        }
        block |= 4LL << 8 * len;
        buff[0].low ^= block;

        if (*result == nullptr) {
            *result = (char*) narcutils::mem::alloc(4 * sizeof(uint96_t));
        }

        uint96_t* datahash = nullptr;
        sponge(buff, &datahash);
        *((uint96_t*) (*result + 0)) = datahash[0];
        *((uint96_t*) (*result + 12)) = datahash[1];

        memcpy(buff, datahash, 6 * sizeof(uint96_t));
        free(datahash);

        datahash = nullptr;
        sponge(buff, &datahash);
        *((uint96_t*) (*result + 24)) = datahash[0];
        *((uint96_t*) (*result + 36)) = datahash[1];

        free(datahash);
        free(buff);
    }

    EncryptedAPI void generate_challenge_pt1(char* body, arc_salt_t salt, uint64_t ts, char** result) {
        size_t body_len = strlen(body);

        size_t buffer_len = sizeof(uint64_t) + body_len + sizeof(arc_salt_t);
        char* buff = (char*) narcutils::mem::alloc(buffer_len);

        *((uint64_t*) buff) = ts;
        memcpy(buff + sizeof(uint64_t), body, body_len);
        memcpy(buff + sizeof(uint64_t) + body_len, salt, sizeof(arc_salt_t));

        hash((unsigned char*) buff, buffer_len, result);

        free(buff);
    }

    EncryptedAPI void generate_challenge_pt2(char* body, char* path, arc_salt_t salt, uint64_t ts, char** result) {
        size_t body_len = strlen(body);
        size_t path_len = strlen(path);

        size_t buffer_len = sizeof(uint64_t) + body_len + path_len + sizeof(arc_salt_t);
        char* buff = (char*) narcutils::mem::alloc(buffer_len);

        *((uint64_t*) buff) = ts;
        memcpy(buff + sizeof(uint64_t), body, body_len);
        memcpy(buff + sizeof(uint64_t) + body_len, path, path_len);
        memcpy(buff + sizeof(uint64_t) + body_len + path_len, salt, sizeof(arc_salt_t));

        hash((unsigned char*) buff, buffer_len, result);

        free(buff);
    }

    HookAPI uint8_t generate_challenge(char* path, char* body, uint64_t ts, arc_salt_t salt, char** result);
    HookAPI uint8_t verify_challenge(char* challenge, char* path, char* body, uint64_t ts, arc_salt_t salt);

    HookAPI uint32_t get_api_version();
}

#endif//NARCHOOK_NARCHASH_H
