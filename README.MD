# narchook

Native ARCaea HOOK

**This project is not open-source.  Any dissemination, distribution, copying, or use of this information is strictly prohibited without the explicit permission of the owner of this repository**

---

## Info

### Features Included

- [x] Disable SSL Pinning
- [x] Custom API Address
- [x] Fake device ID
- [x] **NOT TESTED YET** Generic hooking

### Changelog

- Dev 01
    - Added SSL Pinning Bypass
    - Added Custom API Address Override
- Dev 04
    - Added Fake Device ID
- Dev 05
    - Added Generic Hooking

### Build

Build with cmake + ndk, no support for armeabi-v7a

```shell
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a
```

- Outputs:
  + `libnarchook.so` - China version
  + `libnarchook.so` - Playstore version

## Build Options
Checkout CMakeLists.txt
```cmake
###########################
#    COMPILE SETTINGS     #
###########################

set(TARGET_ARCAEA_VERSION              "5.1.3") # string

set(ENABLE_GENERIC_HOOKING                   1) # boolean   Set to 1 to enable generic hooking
set(OVERRIDE_OFFSET_VSETOPT_PLAYSTORE         ) # size_t    Set a value to override Playstore version default 'CURL_vsetopt' offset
set(OVERRIDE_OFFSET_VSETOPT_CHINA             ) # size_t    Set a value to override China version default 'CURL_vsetopt' offset
set(JNI_SET_DEVICEID_OFFSET_PLAYSTORE         ) # size_t    Set a value to use offset instead of symbol name for 'setDeviceId' hook
set(JNI_SET_DEVICEID_OFFSET_CHINA             ) # size_t    Set a value to use offset instead of symbol name for 'setDeviceId' hook
set(OVERRIDE_JNI_SET_DEVICEID_NAME            ) # string    Set a value to override 'setDeviceId' symbol name

set(NO_PLAYSTORE_SUPPORT                     0) # boolean   Set to 1 to disable Playstore version support
set(NO_CHINA_SUPPORT                         0) # boolean   Set to 1 to disable China version support
set(NO_FAKE_DEVICEID_SUPPORT                 0) # boolean   Set to 1 to disable fake device id support
```

## TOODs

- [x] Fix potential memory leak in `narchook.cpp` (line 43)
- [x] Test if this is working
- [ ] Test Generic Hooking
- [ ] Test Java API
- [ ] Add full documentation since I am forgetting everything