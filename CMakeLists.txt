cmake_minimum_required(VERSION 3.26)
project(narchook CXX)

###########################
#    COMPILE SETTINGS     #
###########################

set(NARCHOOK_API_VERSION                    12) # integer   NArcHook API version
set(NARCHASH_API_VERSION                     2) # integer   NArcHash API version
set(NARCHOOK_BUILD_TYPE                      1) # integer   Set to 1 for release builds
set(NARCHOOK_BUILD_PLATFORM                  1) # integer   0 -> LSPosed, 1 -> amd64-windows, 2 -> amd64-linux
set(NARCHOOK_BUILD_JAVA_SUPPORT              1) # integer   0 -> false, 1 -> true
                                                # In order to build with Java Support, please set environment var:
                                                # NARCHOOK_JAVA_HOME to your JDK installation
###########################
#    COMMON SETTINGS      #
###########################

set(CMAKE_CXX_STANDARD 17)

set(COMMON_INCLUDES common/includes )
set(NARCHOOK_INCLUDES ${COMMON_INCLUDES} narchook/includes)
set(NARCHASH_INCLUDES ${COMMON_INCLUDES} narchash/includes)

if(NARCHOOK_BUILD_JAVA_SUPPORT EQUAL 1)
    set(NARCHOOK_JAVA_HOME "$ENV{NARCHOOK_JAVA_HOME}")
    if (NARCHOOK_JAVA_HOME STREQUAL "")
        message(FATAL_ERROR "Java Support enabled but no JDK installation was found!")
    endif ()

    if(WIN32)
        set(NARCHASH_INCLUDES ${NARCHASH_INCLUDES} ${NARCHOOK_JAVA_HOME}/include/win32 )
    else ()
        set(NARCHASH_INCLUDES ${NARCHASH_INCLUDES} ${NARCHOOK_JAVA_HOME}/include/linux )
    endif ()
    set(NARCHASH_INCLUDES ${NARCHASH_INCLUDES} ${NARCHOOK_JAVA_HOME}/include )
endif ()

set(SOURCES_COMMON common/sources/mem.cpp
        common/sources/utils.cpp
        common/sources/base64.cpp
        common/sources/ada.cpp)

set(XPOSED_SOURCES ${SOURCES_COMMON}
        narchook/sources/javactl.cpp
        narchook/sources/xposedinit.cpp
        narchook/sources/hooks/curl_hacks.cpp
        narchook/sources/hooks/fake_deviceid.cpp
        narchook/sources/hooks/net_debug.cpp
        narchook/sources/hookapi.cpp)

set(NARCHASH_SOURCES ${SOURCES_COMMON}
        narchash/sources/uint96.cpp
        narchash/sources/narchash.cpp
        narchash/sources/externapi/japi.cpp
        narchash/sources/externapi/ffi.cpp)

add_compile_definitions(NARCHOOK_BUILD_VARIANT=${NARCHOOK_BUILD_TARGET})
add_compile_definitions(NARCHOOK_API_VERSION=${NARCHOOK_API_VERSION})
add_compile_definitions(NARCHASH_API_VERSION=${NARCHASH_API_VERSION})
add_compile_definitions(NARCHOOK_BUILD_TYPE=${NARCHOOK_BUILD_TYPE})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NARCHOOK_BUILD_RELEASE=1)
    add_compile_options(-O3 -g0 -fvisibility=hidden)
else()
    add_compile_definitions(NARCHOOK_BUILD_DEBUG=1)
    add_compile_options(-O0)
endif ()

if (NARCHOOK_BUILD_PLATFORM EQUAL 0)
add_library(narchook SHARED ${XPOSED_SOURCES})
set_target_properties(narchook PROPERTIES OUTPUT_NAME narchook)
target_include_directories(narchook PRIVATE ${NARCHOOK_INCLUDES})

set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM 30)
endif ()


add_library(narchash SHARED ${NARCHASH_SOURCES})
set_target_properties(narchash PROPERTIES OUTPUT_NAME narchash)
target_include_directories(narchash PRIVATE ${NARCHASH_INCLUDES})


if (NARCHOOK_BUILD_PLATFORM EQUAL 0)
    find_library(logcat log)
    target_link_libraries(narchook ${logcat})
    add_compile_definitions(NARCHOOK_PLATFORM_ANDROID=1)
    add_compile_definitions(NARCHOOK_PLATFORM_CLANG=1)
elseif(NARCHOOK_BUILD_PLATFORM EQUAL 1)
    add_compile_definitions(NARCHOOK_PLATFORM_MSVC=1)
else ()
    add_compile_definitions(NARCHOOK_PLATFORM_CLANG=1)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Now building narchook (R${NARCHOOK_API_VERSION}) on ${CMAKE_HOST_SYSTEM_NAME}")
    if (CMAKE_HOST_WIN32)
        if (NARCHOOK_BUILD_PLATFORM EQUAL 0)
            add_custom_command(TARGET narchook POST_BUILD
                    COMMAND powershell.exe
                    ARGS -File ${CMAKE_CURRENT_SOURCE_DIR}/minify.ps1 $<TARGET_FILE:narchook>
                    COMMENT "Running minify script..."
            )
        endif ()

        add_custom_command(TARGET narchash POST_BUILD
                COMMAND powershell.exe
                ARGS -File ${CMAKE_CURRENT_SOURCE_DIR}/minify.ps1 $<TARGET_FILE:narchash>
                COMMENT "Running minify script..."
        )
    else ()
        if (NARCHOOK_BUILD_PLATFORM EQUAL 0)
            add_custom_command(TARGET narchook POST_BUILD
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/minify.sh
                ARGS $<TARGET_FILE:narchook>
                COMMENT "Running minify script..."
            )
        endif ()

        add_custom_command(TARGET narchash POST_BUILD
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/minify.sh
                ARGS $<TARGET_FILE:narchash>
                COMMENT "Running minify script..."
        )
    endif ()
else()
    message("Now building narchook + narchash (D${NARCHOOK_API_VERSION}) on ${CMAKE_HOST_SYSTEM_NAME}")
endif ()