cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(relive VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_program(CMAKE cmake)
if(NOT CMAKE)
    message(FATAL_ERROR "CMake binary not found!")
endif()

if (NOT CI_PROVIDER)
    set(CI_PROVIDER "private (local build)")
endif()

# From: https://medium.com/@alasher/colored-c-compiler-output-with-ninja-clang-gcc-10bfe7f2b949
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

add_custom_target(
    UpdateConfigHeader
    COMMAND "${CMAKE}" -D BUILD_NUMBER=${BUILD_NUMBER} -D CI_PROVIDER="${CI_PROVIDER}" -P ${CMAKE_CURRENT_LIST_DIR}/options.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  string(REPLACE "/W3" "" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

if(MSVC)
    add_compile_options(/bigobj)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

if (NOT DEFINED ENV{AE_ROOT})
    message(WARNING "No AE_ROOT environment variable found, resorting to the default path")
    set(AE_PATH "C:\\GOG Games\\Abes Exoddus" )
else()
    file(TO_CMAKE_PATH $ENV{AE_ROOT} AE_PATH)
endif()

if (NOT DEFINED ENV{AO_ROOT})
    message(WARNING "No AO_ROOT environment variable found, resorting to the default path")
    set(AO_PATH "C:\\GOG Games\\Abes Oddysee" )
else()
    file(TO_CMAKE_PATH $ENV{AO_ROOT} AO_PATH)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/3rdParty)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/AliveLibCommon)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/AliveLibAO)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/AliveLibAE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/relive)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools/vab_tool)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools/relive_api)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools/relive_api_integration_test)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools/relive_api_unit_test)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/assets)

# -----------------------------------------------------------------------------
# Setup unity builds
set_target_properties(EasyLogging++ PROPERTIES UNITY_BUILD ON)
set_target_properties(googletest    PROPERTIES UNITY_BUILD ON)
set_target_properties(imgui         PROPERTIES UNITY_BUILD ON)

# Disable for now because it breaks alivehook manager by setting unity_XX 
# to always be the "group"
set_target_properties(AliveLibCommon PROPERTIES UNITY_BUILD OFF)
set_target_properties(AliveLibAE     PROPERTIES UNITY_BUILD OFF)
set_target_properties(AliveLibAO     PROPERTIES UNITY_BUILD OFF)
set_target_properties(relive         PROPERTIES UNITY_BUILD OFF)

set_target_properties(relive_api      PROPERTIES UNITY_BUILD OFF) # too big
set_target_properties(relive_api_integration_test PROPERTIES UNITY_BUILD OFF) # too big
set_target_properties(relive_api_unit_test PROPERTIES UNITY_BUILD OFF) # too big

# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Setup Gand Clang warnings
add_library(project_warnings INTERFACE)
export(TARGETS project_warnings FILE project_warnings.cmake)

if(NOT MSVC)
    include(cmake/CompilerWarnings.cmake)
    set_project_warnings(project_warnings)
endif()
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# Setup cross-platform PCH

# AE needs its own PCH that can be reused from `AliveLibCommon`, because they
# both define the same values for `BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE`,
# `BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS`, and `_CRT_SECURE_NO_WARNINGS'
target_precompile_headers(
    AliveLibCommon PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/AliveLibCommon/pch_shared.h"
)

target_precompile_headers(AliveLibAE REUSE_FROM AliveLibCommon)

# AO needs its own PCH because of different `BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE`
# and `BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS` defines with `AliveLibCommon`
target_precompile_headers(
    AliveLibAO PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/AliveLibCommon/pch_shared.h"
)

target_precompile_headers(relive_api_integration_test REUSE_FROM AliveLibAO)
target_precompile_headers(relive_api_unit_test REUSE_FROM AliveLibAO)
target_precompile_headers(relive       REUSE_FROM AliveLibAO)

# 'relive_api' needs its own PCH because of different `_CRT_SECURE_NO_WARNINGS`
# and defines with the rest
target_precompile_headers(
    relive_api PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/AliveLibCommon/pch_shared.h"
)

if(WIN32)
    target_precompile_headers(vab_tool REUSE_FROM AliveLibAO)
endif()
# -----------------------------------------------------------------------------

add_dependencies(AliveLibCommon UpdateConfigHeader)

if(WIN32)
    # On Windows generate ZIP packages
    SET(CPACK_GENERATOR "ZIP")
endif()

SET(CPACK_PACKAGE_CONTACT "nemin@oddwords.hu")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://aliveteam.github.io/")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0, zenity")

# bash is required to run install hooks
set(CPACK_RPM_RELIVE_REQUIRES_PRE "bash")

# package name
set(CPACK_RPM_RELIVE_PACKAGE_NAME "relive")

set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
  /usr/share/applications
  /usr/share/pixmaps
  /usr/include/GL
)

INCLUDE(CPack)
