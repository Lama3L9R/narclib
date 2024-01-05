cmake_minimum_required(VERSION 3.26)
project(narchook)

###########################
#    COMPILE SETTINGS     #
###########################

set(NARCHOOK_API_VERSION                    10) # integer   NArcHook API version
set(NARCHOOK_BUILD_TYPE                      1) # integer   Set to 1 for release builds
set(NARCHOOK_BUILD_TARGET                    0) # integer   0 -> LSPosed, 1 -> Standalone library

###########################
#    COMMON SETTINGS      #
###########################

set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM 30)
set(CMAKE_CXX_STANDARD 17)

include_directories(includes)

set(SOURCES jni/japi.cpp
        jni/hookapi.cpp
        jni/mem.cpp
        jni/utils.cpp)

if (NARCHOOK_BUILD_TARGET EQUAL 0)
    set(SOURCES ${SOURCES}
            jni/narchook.cpp
            jni/ada.cpp
            jni/hooks/curl_hacks.cpp
            jni/hooks/fake_deviceid.cpp
            jni/hooks/net_debug.cpp)
endif()

add_compile_definitions(NARCHOOK_API_VERSION=${NARCHOOK_API_VERSION})
add_compile_definitions(NARCHOOK_BUILD_TYPE=${NARCHOOK_BUILD_TYPE})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NARCHOOK_BUILD_RELEASE=1)
    add_compile_options(-O3 -fvisibility=hidden)
else()
    add_compile_definitions(NARCHOOK_BUILD_DEBUG=1)
    add_compile_options(-O0)
endif ()

find_library(logcat log)
add_library(narchook SHARED ${SOURCES})
target_link_libraries(narchook ${logcat})
set_target_properties(narchook PROPERTIES OUTPUT_NAME narchook)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Now building narchook (R${NARCHOOK_API_VERSION}) on ${CMAKE_HOST_SYSTEM_NAME}")
    if (CMAKE_HOST_WIN32)
        add_custom_command(TARGET narchook POST_BUILD
                COMMAND powershell.exe
                ARGS -File ${CMAKE_CURRENT_SOURCE_DIR}/minify.ps1 $<TARGET_FILE:narchook>
                COMMENT "Running minify script..."
        )
    else ()
        add_custom_command(TARGET narchook POST_BUILD
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/minify.sh
                ARGS $<TARGET_FILE:narchook>
                COMMENT "Running minify script..."
        )
    endif ()
else()
    message("Now building narchook (D${NARCHOOK_API_VERSION}) on ${CMAKE_HOST_SYSTEM_NAME}")
endif ()