cmake_minimum_required(VERSION 3.20)

project(SCAPI CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Fetch modules
message("\nFetching modules")
include(scripts/pugixml.cmake)
include(scripts/cpr.cmake)
include(scripts/json.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
find_package(botan 2.19.3 REQUIRED)
botan_generate(botan sha1 hmac rc4 rng base64 mac stream hash auto_rng win32_stats)

# SCAPI
message(STATUS "SCAPI")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)
add_executable(SCAPI "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET SCAPI PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN} )

target_include_directories(SCAPI PRIVATE 
    "${SRC_DIR}"
    "${json_SOURCE_DIR}/single_include"
)

target_link_libraries(SCAPI PRIVATE botan pugixml cpr json)

# Warnings as errors
set_property(TARGET SCAPI PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(SCAPI 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)