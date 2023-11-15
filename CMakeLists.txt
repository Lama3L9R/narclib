cmake_minimum_required(VERSION 3.26)
project(narchook)

###########################
#    COMPILE SETTINGS     #
###########################

set(TARGET_ARCAEA_VERSION              "5.1.3") # string

set(OVERRIDE_OFFSET_VSETOPT_PLAYSTORE         ) # size_t    Set a value to override Playstore version default 'CURL_vsetopt' offset
set(OVERRIDE_OFFSET_VSETOPT_CHINA             ) # size_t    Set a value to override China version default 'CURL_vsetopt' offset
set(JNI_SET_DEVICEID_OFFSET_PLAYSTORE         ) # size_t    Set a value to use offset instead of symbol name for 'setDeviceId' hook
set(JNI_SET_DEVICEID_OFFSET_CHINA             ) # size_t    Set a value to use offset instead of symbol name for 'setDeviceId' hook
set(OVERRIDE_JNI_SET_DEVICEID_NAME            ) # string    Set a value to override 'setDeviceId' symbol name

set(NO_PLAYSTORE_SUPPORT                     0) # boolean   Set to 1 to disable Playstore version support
set(NO_CHINA_SUPPORT                         0) # boolean   Set to 1 to disable China version support
set(NO_FAKE_DEVICEID_SUPPORT                 0) # boolean   Set to 1 to disable fake device id support

###########################
#    COMMON SETTINGS      #
###########################

set(CMAKE_CXX_STANDARD 14)

include_directories(jni)

set(SOURCES jni/narchook.cpp
        jni/utils.cpp
        jni/japi.cpp
        jni/offset.cpp)

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

message("Now building narchook for Arcaea version: ${TARGET_ARCAEA_VERSION}")
add_library(narchook SHARED ${SOURCES})
target_compile_definitions(narchook PUBLIC USE_CHINA_VER TARGET_VERSION="\\"${TARGET_ARCAEA_VERSION}\\"")
set_target_properties(narchook PROPERTIES OUTPUT_NAME narchook)