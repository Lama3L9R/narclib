cmake_minimum_required(VERSION 3.26)
project(narchook)

###########################
#    COMPILE SETTINGS     #
###########################

set(TARGET_ARCAEA_VERSION                5.2.0) # string

set(ENABLE_GENERIC_HOOKING                   1) # boolean   Set to 1 to enable generic hooking
set(OVERRIDE_OFFSET_VSETOPT_PLAYSTORE         ) # size_t    Set a value to override Playstore version default 'CURL_vsetopt' offset
set(OVERRIDE_OFFSET_VSETOPT_CHINA             ) # size_t    Set a value to override China version default 'CURL_vsetopt' offset
set(JNI_SET_DEVICEID_OFFSET_PLAYSTORE         ) # size_t    Set a value to use offset instead of symbol name for 'setDeviceId' hook
set(JNI_SET_DEVICEID_OFFSET_CHINA             ) # size_t    Set a value to use offset instead of symbol name for 'setDeviceId' hook
set(OVERRIDE_JNI_SET_DEVICEID_NAME            ) # string    Set a value to override 'setDeviceId' symbol name

set(NO_PLAYSTORE_SUPPORT                     0) # boolean   Set to 1 to disable Playstore version support
set(NO_CHINA_SUPPORT                         0) # boolean   Set to 1 to disable China version support
set(NO_FAKE_DEVICEID_SUPPORT                 0) # boolean   Set to 1 to disable fake device id support

set(NARCHOOK_API_VERSION                     9) # integer   NArcHook API version
set(NARCHOOK_BUILD_TYPE                      0) # integer   Set to 1 for release builds

###########################
#    COMMON SETTINGS      #
###########################

set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM 30)
set(CMAKE_CXX_STANDARD 17)

include_directories(includes)

set(SOURCES jni/narchook.cpp
        jni/japi.cpp
        jni/ada.cpp
        jni/hookapi.cpp
        jni/mem.cpp
        jni/hooks/curl_hacks.cpp
        jni/utils.cpp
        jni/hooks/fake_deviceid.cpp)

if (OVERRIDE_OFFSET_VSETOPT_PLAYSTORE)
    add_compile_definitions(OVERRIDE_OFFSET_VSETOPT_PLAYSTORE=${OVERRIDE_OFFSET_VSETOPT_PLAYSTORE})
    message("Overriding playstore version default 'CURL_vsetopt' offset with: ${OVERRIDE_OFFSET_VSETOPT_PLAYSTORE}")
endif()

if (OVERRIDE_OFFSET_VSETOPT_CHINA)
    add_compile_definitions(OVERRIDE_OFFSET_VSETOPT_CHINA=${OVERRIDE_OFFSET_VSETOPT_CHINA})
    message("Overriding China version default 'CURL_vsetopt' offset with: ${OVERRIDE_OFFSET_VSETOPT_CHINA}")
endif()

if (JNI_SET_DEVICEID_OFFSET_PLAYSTORE)
    add_compile_definitions(JNI_SET_DEVICEID_OFFSET_PLAYSTORE=${JNI_SET_DEVICEID_OFFSET_PLAYSTORE})
    message(WARNING "Using offset instead of symbol name for 'setDeviceId' hook in Playstore version: ${JNI_SET_DEVICEID_OFFSET_PLAYSTORE}")
endif()

if (JNI_SET_DEVICEID_OFFSET_CHINA)
    add_compile_definitions(JNI_SET_DEVICEID_OFFSET_CHINA=${JNI_SET_DEVICEID_OFFSET_CHINA})
    message(WARNING "Using offset instead of symbol name for 'setDeviceId' hook in China version: ${JNI_SET_DEVICEID_OFFSET_CHINA}")
endif()

if (OVERRIDE_JNI_SET_DEVICEID_NAME)
    add_compile_definitions(OVERRIDE_JNI_SET_DEVICEID_NAME="\\"${OVERRIDE_JNI_SET_DEVICEID_NAME}\\"")
    message("Overriding 'setDeviceId' symbol name with: ${OVERRIDE_JNI_SET_DEVICEID_NAME}")
endif()

if (NO_PLAYSTORE_SUPPORT)
    add_compile_definitions(NO_PLAYSTORE_SUPPORT)
    message(WARNING "Playstore version support disabled!")
endif()

if (NO_CHINA_SUPPORT)
    add_compile_definitions(NO_CHINA_SUPPORT)
    message(WARNING "China version support disabled!")
endif()

if (NO_FAKE_DEVICEID_SUPPORT)
    add_compile_definitions(NO_FAKE_DEVICEID_SUPPORT)
    message(WARNING "Fake device id support disabled!")
endif()

if (ENABLE_GENERIC_HOOKING)
    add_compile_definitions(ENABLE_GENERIC_HOOKING)
    message(WARNING "Generic hooking enabled!")
endif()

add_compile_definitions(NARCHOOK_API_VERSION=${NARCHOOK_API_VERSION})
add_compile_definitions(NARCHOOK_BUILD_TYPE=${NARCHOOK_BUILD_TYPE})

if (NARCHOOK_BUILD_TYPE EQUAL 1)
    message("Now building narchook (R${NARCHOOK_API_VERSION}) for Arcaea version: ${TARGET_ARCAEA_VERSION}")
    add_compile_options(-O3)
else()
    message("Now building narchook (D${NARCHOOK_API_VERSION}) for Arcaea version: ${TARGET_ARCAEA_VERSION}")
    add_compile_options(-O0)
endif ()

find_library(logcat log)
add_library(narchook SHARED ${SOURCES})
target_link_libraries(narchook ${logcat})
target_compile_definitions(narchook PUBLIC USE_CHINA_VER TARGET_VERSION="\\"${TARGET_ARCAEA_VERSION}\\"")
set_target_properties(narchook PROPERTIES OUTPUT_NAME narchook)