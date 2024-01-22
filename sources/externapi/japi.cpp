//
// Created by lamadaemon on 1/18/2024.
//

#include "jni.h"
#include "narchook.h"

extern "C" JNIEXPORT jint JNICALL Java_icu_lama_narchook_NArcHook_getVersion(JNIEnv* env, jclass clazz) {
    return narchook::get_api_version();
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_icu_lama_narchook_NArcHook_generateChallenge(JNIEnv* env, jclass clazz,
                                                                                    jstring path,jstring body,jlong timestamp,jbyteArray salt) {
    char*    path_c = (char*) env->GetStringUTFChars(path, 0);
    char*    body_c = (char*) env->GetStringUTFChars(body, 0);
    uint8_t* salt_c = (uint8_t*) env->GetByteArrayElements(salt, 0);

    char*   result;
    uint8_t err;

    if ((err = narchook::archash::generate_challenge(path_c, body_c, timestamp, salt_c, &result)) != GENERATE_SUCCESS) {
        jclass exception = env->FindClass("java/lang/RuntimeException");

        if (exception == NULL) {
            return env->NewByteArray(0);
        }

        char msg[45] = { 0 };
        snprintf(msg, 40, "Failed to generate challenge string (%d)", err);
        env->ThrowNew(exception, msg);

        return NULL;
    }

    jbyteArray rtn = env->NewByteArray(narchook::archash::challenge_size());
    env->SetByteArrayRegion(rtn, 0, narchook::archash::challenge_size(), (const jbyte*) result);

    env->ReleaseStringUTFChars(path, path_c);
    env->ReleaseStringUTFChars(body, body_c);
    env->ReleaseByteArrayElements(salt, (jbyte*) salt_c, JNI_ABORT);

    return rtn;
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_NArcHook_verifyChallenge(JNIEnv* env, jclass clazz,
                                                                                  jstring path, jstring body, jlong timestamp, jbyteArray salt, jbyteArray challenge) {
    char*    path_c      = (char*) env->GetStringUTFChars(path, 0);
    char*    body_c      = (char*) env->GetStringUTFChars(body, 0);
    uint8_t* salt_c      = (uint8_t*) env->GetByteArrayElements(salt, 0);
    char*    challenge_c = (char*) env->GetByteArrayElements(challenge, 0);

    uint8_t result;
    if ((result = narchook::archash::verify_challenge(challenge_c, path_c, body_c, timestamp, salt_c)) != VERIFY_NO_ERROR) {
        jclass exception = env->FindClass("java/lang/RuntimeException");

        if (exception == NULL) {
            return;
        }

        char msg[45] = { 0 };
        snprintf(msg, 40, "Failed to verify challenge (%d)", result);
        env->ThrowNew(exception, msg);

        return;
    }
}