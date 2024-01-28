#include "narchash.h"
#include "narcutils.h"

using namespace narcutils::utils;

namespace narchash {

    uint8_t generate_challenge(char* path, char* body, uint64_t ts, arc_salt_t salt, char** result) {
        uint64_t timestamp = ts;
        if (!timestamp) {
            timestamp = get_ts_now();

            if (!timestamp) {
                return GENERATE_FAILED_GETTIME;
            }
        }

        char* buff = (char*) narcutils::mem::alloc(challenge_size());
        *((uint64_t*) buff) = timestamp;

        char* buff_pt1 = buff + sizeof(uint64_t);
        char* buff_pt2 = buff_pt1 + HASH_SIZE;
        generate_challenge_pt1(body, path, salt, timestamp, &buff_pt1);
        generate_challenge_pt2(body, salt, timestamp, &buff_pt2);

        *result = (char*) base64::encode((uint8_t*) buff, challenge_size(), nullptr);

        return GENERATE_SUCCESS;
    }

    uint8_t verify_challenge(char* challenge, char* path, char* body, uint64_t ts, arc_salt_t salt) {
        size_t raw_challenge_len;
        uint8_t* raw_challenge = base64::decode((const uint8_t *)challenge, strlen(challenge), &raw_challenge_len);
        if (raw_challenge_len != challenge_size()) {
            return VERIFY_ILLEGAL_FORMAT;
        }

        uint64_t timestamp_given = *((uint64_t*) raw_challenge);
        if (timestamp_given + 30000 < get_ts_now()) {
            return VERIFY_OUTDATED;
        }

        char* generated;
        generate_challenge(path, body, timestamp_given, salt, &generated);

        if (memcmp(generated, raw_challenge, challenge_size())) {
            return VERIFY_ILLEGAL_HASH;
        }

        return VERIFY_NO_ERROR;
    }

    HookAPI uint32_t get_api_version() {
        return 1; // TODO
    }
}