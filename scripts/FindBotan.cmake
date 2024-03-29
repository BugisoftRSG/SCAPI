## This module will automagically download the tarball of the specified Botan version and invoke the configure.py
## python script to generate the amalgamation files (botan_all.cpp and botan_all.h).
##
## Usage:
##   find_package(
##       botan 2.18.2
##       COMPONENTS
##           system_rng
##           argon2
##           sha3
##       REQUIRED
##    )
##
##    target_link_libraries(
##        MyTarget
##        PRIVATE
##            botan
##    )
##

cmake_minimum_required(VERSION 3.19)
include(FetchContent)

# Find python
find_package(
    Python
    COMPONENTS
        Interpreter
    REQUIRED
)

# Assemble version string
set(Botan_VERSION_STRING "Botan-2.19.3")

# Assemble download URL
set(DOWNLOAD_URL https://botan.randombit.net/releases/${Botan_VERSION_STRING}.tar.xz)

# Just do a dummy download to see whether we can download the tarball
file(
    DOWNLOAD
    ${DOWNLOAD_URL}
    STATUS download_status
)
if (NOT download_status EQUAL 0)
    message(FATAL_ERROR "Could not download Botan tarball (status = ${download_status}): ${DOWNLOAD_URL}")
endif()

# Download the tarball
FetchContent_Declare(
    botan_upstream
    URL ${DOWNLOAD_URL}
)
FetchContent_MakeAvailable(botan_upstream)

# Heavy lifting by cmake
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Botan DEFAULT_MSG Botan_VERSION_STRING)

## Function to generate a target named 'TARGET_NAME' with specific Botan modules enabled.
function(botan_generate TARGET_NAME MODULES)
    # The last N arguments are considered to be the modules list.
    # Here, we collect those in a list and join them with a comma separator ready to be passed to the configure.py script.
    foreach(module_index RANGE 1 ${ARGC}-2)
        list(APPEND modules_list ${ARGV${module_index}})

        # Check if PKCS11 module is enabled
        # Note: This is for a workaround, see further below for more details.
        if (ARGV${module_index} STREQUAL "pkcs11")
            set(PKCS11_ENABLED ON)
        endif()
    endforeach()
    list(JOIN modules_list "," ENABLE_MODULES_LIST)

    # Determine botan compiler ID (--cc parameter of configure.py)
    set(BOTAN_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
    string(TOLOWER ${BOTAN_COMPILER_ID} BOTAN_COMPILER_ID)
    if (BOTAN_COMPILER_ID STREQUAL "gnu")
        set(BOTAN_COMPILER_ID "gcc")
    endif()

    # Run the configure.py script
    add_custom_command(
        OUTPUT botan_all.cpp botan_all.h
        COMMENT "Generating Botan amalgamation files botan_all.cpp and botan_all.h"
        COMMAND ${Python_EXECUTABLE}
            ${botan_upstream_SOURCE_DIR}/configure.py
            --quiet
            --cc-bin=${CMAKE_CXX_COMPILER}
            --cc=${BOTAN_COMPILER_ID}
            $<$<BOOL:${MINGW}>:--os=mingw>
            --disable-shared
            --amalgamation
            --minimized-build
            --enable-modules=${ENABLE_MODULES_LIST}
    )

    # Create target
    set(TARGET ${TARGET_NAME})
    add_library(${TARGET} STATIC)
    target_sources(
        ${TARGET}
        PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR}/botan_all.h
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/botan_all.cpp
    )

    target_include_directories(
        ${TARGET}
        INTERFACE
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    set_target_properties(
        ${TARGET}
        PROPERTIES
            POSITION_INDEPENDENT_CODE ON
    )

    #
    # PKCS11 Workaround
    #
    # This section is a workaround to handle a "bug" in upstream Botan.
    # Basically, the amalgamation build of Botan does not include the necessary PKCS11 headers when the PKCS11 module
    # is enabled.
    #
    # See:
    #   - https://github.com/randombit/botan/issues/1447
    #   - https://github.com/randombit/botan/issues/976
    #
    if (PKCS11_ENABLED)
        file(COPY ${botan_upstream_SOURCE_DIR}/src/lib/prov/pkcs11/pkcs11.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        file(COPY ${botan_upstream_SOURCE_DIR}/src/lib/prov/pkcs11/pkcs11f.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        file(COPY ${botan_upstream_SOURCE_DIR}/src/lib/prov/pkcs11/pkcs11t.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        target_include_directories(
                ${TARGET}
                PRIVATE
                ${botan_upstream_SOURCE_DIR}/src/lib/prov/pkcs11
        )
    endif()
endfunction()