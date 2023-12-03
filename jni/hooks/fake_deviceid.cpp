//
// Created by lamadaemon on 12/2/2023.
//
#include "hooks/fake_deviceid.h"
#include "log.h"
#include <jni.h>

void *(*Arc_Game_setDeviceId)(JNIEnv* env, jclass clazz, jstring id);

static char* override_id = nullptr;
static bool ended = false;

void *Arc_Game_setDeviceId_callback(JNIEnv* env, jclass clazz, jstring id) {
    if (ended) {
        return Arc_Game_setDeviceId(env, clazz, id);
    }

    if (!narchook::is_feature_enabled(FEAT_FAKEDEVICEID)) {
        return Arc_Game_setDeviceId(env, clazz, id);
    }

    if (override_id != nullptr) {
        LOGI("Setting device id to %s (override)", override_id);
        return Arc_Game_setDeviceId(env, clazz, env->NewStringUTF(override_id));
    }
    LOGI("Setting device id to %s", env->GetStringUTFChars(id, JNI_FALSE));

    return Arc_Game_setDeviceId(env, clazz, id);
}

namespace narchook::hooks::deviceid {

    void set_param(uint32_t param, ...) {
        va_list args;
        va_start(args, param);

        switch (param) {
            case PARAM_FAKEDEVICEID_NEWID:
                if (override_id != nullptr) {
                    free(override_id);
                }
                override_id = strdup(va_arg(args, char*));

                break;
        }

        va_end(args);
    }

    hooking_feature_t begin() {
        return hooking_feature_t {
            .feature = FEAT_FAKEDEVICEID,
            .is_enabled = false,
            .hooking_method = HOOKING_USE_EXPORT_NAME,
            .params = { PATTERN_NOT_SUPPORTED, OFFSET_NOT_SUPPORTED, hooking_param {
                .is_supported = true,
                .method = HOOKING_USE_EXPORT_NAME,
                .target = hooking_target_t {
                    .named = {
                        .name = strdup("Java_low_moe_AppActivity_setDeviceId"),
                    }
                }
            } },
            .handle_hook = (void*) Arc_Game_setDeviceId_callback,
            .original_fn = (void**) &Arc_Game_setDeviceId
        };
    }

    void end() {
        if (override_id != nullptr) {
            free(override_id);
        }

        ended = true;
    }
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_FakeDeviceID_setFakeDeviceID(JNIEnv *env, jclass clazz, jstring newID) {
    const char* id = env->GetStringUTFChars(newID, JNI_FALSE);
    narchook::hooks::deviceid::set_param(PARAM_FAKEDEVICEID_NEWID, strdup(id));
    env->ReleaseStringUTFChars(newID, id);
}

extern "C" JNIEXPORT void JNICALL Java_icu_lama_narchook_FakeDeviceID_toggleFeature(JNIEnv* env, jclass clazz, jboolean enabled) {
    narchook::set_feature_enabled(FEAT_FAKEDEVICEID, enabled);
}