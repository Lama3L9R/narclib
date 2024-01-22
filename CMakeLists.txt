cmake_minimum_required(VERSION 3.26)
project(narchook CXX)

###########################
#    COMPILE SETTINGS     #
###########################

set(NARCHOOK_API_VERSION                    11) # integer   NArcHook API version
set(NARCHOOK_BUILD_TYPE                      1) # integer   Set to 1 for release builds
set(NARCHOOK_BUILD_TARGET                    1) # integer   0 -> LSPosed, 1 -> Standalone library
set(NARCHOOK_BUILD_PLATFORM                  1) # integer   0 -> LSPosed, 1 -> amd64-windows, 2 -> amd64-linux
set(NARCHOOK_BUILD_JAVA_SUPPORT              1) # integer   0 -> true, 1 -> false
                                                # In order to build with Java Support, please set environment var:
                                                # NARCHOOK_JAVA_HOME to your JDK installation
set(NARCHOOK_BUILD_NODE_SUPPORT              0) # integer   0 -> true, 1 -> false
                                                # In order to build with Node Support, please set environment var:
                                                # NARCHOOK_NODE_HOME to your Nodejs installation
###########################
#    COMMON SETTINGS      #
###########################

set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM 30)
set(CMAKE_CXX_STANDARD 17)

set(INCLUDES includes)

if(NARCHOOK_BUILD_JAVA_SUPPORT EQUAL 1)
    set(NARCHOOK_JAVA_HOME "$ENV{NARCHOOK_JAVA_HOME}")
    if (NARCHOOK_JAVA_HOME STREQUAL "")
        message(FATAL_ERROR "Java Support enabled but no JDK installation was found!")
    endif ()

    if(WIN32)
        set(INCLUDES ${INCLUDES} ${NARCHOOK_JAVA_HOME}/include/win32 )
    else ()
        set(INCLUDES ${INCLUDES} ${NARCHOOK_JAVA_HOME}/include/linux )
    endif ()
    set(INCLUDES ${INCLUDES} ${NARCHOOK_JAVA_HOME}/include )
endif ()

if(NARCHOOK_BUILD_NODE_SUPPORT EQUAL 1)
    set(NARCHOOK_JAVA_HOME "$ENV{NARCHOOK_JAVA_HOME}")
    if (NARCHOOK_JAVA_HOME STREQUAL "")
        message(FATAL_ERROR "Java Support enabled but no JDK installation was found!")
    endif ()

    if(WIN32)
        set(INCLUDES ${INCLUDES} ${NARCHOOK_JAVA_HOME}/include/win32 )
    else ()
        set(INCLUDES ${INCLUDES} ${NARCHOOK_JAVA_HOME}/include/linux )
    endif ()
    set(INCLUDES ${INCLUDES} ${NARCHOOK_JAVA_HOME}/include )
endif ()


include_directories(${INCLUDES})

set(SOURCES sources/narchook.cpp
        sources/utils/mem.cpp
        sources/utils/utils.cpp)

if (NARCHOOK_BUILD_TARGET EQUAL 0)
    set(SOURCES ${SOURCES}
            sources/xposed/javactl.cpp
            sources/xposed/xposedinit.cpp
            sources/utils/ada.cpp
            sources/xposed/hooks/curl_hacks.cpp
            sources/xposed/hooks/fake_deviceid.cpp
            sources/xposed/hooks/net_debug.cpp
            sources/xposed/hookapi.cpp)
else()
    set (SOURCES ${SOURCES}
            sources/archash/uint96.cpp
            sources/archash/archash.cpp
            sources/utils/base64.cpp
            sources/externapi/japi.cpp)
endif()

add_compile_definitions(NARCHOOK_BUILD_VARIANT=${NARCHOOK_BUILD_TARGET})
add_compile_definitions(NARCHOOK_API_VERSION=${NARCHOOK_API_VERSION})
add_compile_definitions(NARCHOOK_BUILD_TYPE=${NARCHOOK_BUILD_TYPE})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NARCHOOK_BUILD_RELEASE=1)
    add_compile_options(-O3 -fvisibility=hidden)
else()
    add_compile_definitions(NARCHOOK_BUILD_DEBUG=1)
    add_compile_options(-O0)
endif ()

add_library(narchook SHARED ${SOURCES})
set_target_properties(narchook PROPERTIES OUTPUT_NAME narchook)

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