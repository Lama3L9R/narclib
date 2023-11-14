//
// Created by lamadaemon on 11/13/2023.
//

#ifndef NARCHOOK_OFFSET_H
#define NARCHOOK_OFFSET_H

#define CURLOPT_SSLCERTTYPE 10086
#define CURLOPT_SSLKEYPASSWD 10026
#define CURLOPT_SSLCERT_BLOB 40291
#define CURLOPT_PINNEDPUBLICKEY 10230
#define CURLOPT_SSL_VERIFYPEER 64
#define CURLOPT_URL 10002

#define CURL_SUCCESS 0

const char* libname = "libcocos2dcpp.so";
const uint64_t option_deny_list[] = {CURLOPT_SSLCERTTYPE, CURLOPT_SSLKEYPASSWD, CURLOPT_SSLCERT_BLOB, CURLOPT_PINNEDPUBLICKEY};

#ifdef USE_PLAYSTORE_VER

#define OFFSET_Arc_CURL_vsetopt 0xA3C364

#warning "Using PlayStore version"

#else

#define OFFSET_Arc_CURL_vsetopt 0xED0064

#warning "Using China version"

#endif

#endif //NARCHOOK_OFFSET_H
