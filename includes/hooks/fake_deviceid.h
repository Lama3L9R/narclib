//
// Created by lamadaemon on 12/2/2023.
//

#ifndef NARCHOOK_FAKE_DEVICEID_H
#define NARCHOOK_FAKE_DEVICEID_H

#include "hookapi.h"

#define FEAT_FAKEDEVICEID 1

#define PARAM_FAKEDEVICEID_NEWID 0


namespace narchook::hooks::deviceid {

    void set_param(uint32_t param, ...);

    hooking_feature_t begin();

    void end();
}// namespace narchook::hooks::deviceid

#endif//NARCHOOK_FAKE_DEVICEID_H
