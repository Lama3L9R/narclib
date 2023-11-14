cmake_minimum_required(VERSION 3.26)
project(narchook)

set(CMAKE_CXX_STANDARD 14)

include_directories(jni)

set(SOURCES jni/narchook.cpp
        jni/utils.cpp
        jni/jniconf.cpp)

add_library(narchookc SHARED ${SOURCES})
add_library(narchookp SHARED ${SOURCES})

target_compile_definitions(narchookc PUBLIC USE_CHINA_VER)
set_target_properties(narchookc PROPERTIES OUTPUT_NAME narchookc)

target_compile_definitions(narchookp PUBLIC USE_PLAYSTORE_VER)
set_target_properties(narchookp PROPERTIES OUTPUT_NAME narchookp)
