//
// Created by lamadaemon on 1/16/2024.
//

#ifndef NARCHOOK_BASE64_H
#define NARCHOOK_BASE64_H

#include <cstdint>

namespace narchook::utils::base64 {
    /**
     * encode - Base64 encode
     * @src: Data to be encoded
     * @len: Length of the data to be encoded
     * @out_len: Pointer to output length variable, or %NULL if not used
     * Returns: Allocated buffer of out_len bytes of encoded data,
     * or %NULL on failure
     *
     * Caller is responsible for freeing the returned buffer. Returned buffer is
     * nul terminated to make it easier to use as a C string. The nul terminator is
     * not included in out_len.
     */
    unsigned char* encode(const unsigned char* src, size_t len,
                          size_t* out_len);

    /**
     * decode - Base64 decode
     * @src: Data to be decoded
     * @len: Length of the data to be decoded
     * @out_len: Pointer to output length variable
     * Returns: Allocated buffer of out_len bytes of decoded data,
     * or %NULL on failure
     *
     * Caller is responsible for freeing the returned buffer.
     */
    unsigned char* decode(const unsigned char* src, size_t len,
                          size_t* out_len);
}

#endif//NARCHOOK_BASE64_H
