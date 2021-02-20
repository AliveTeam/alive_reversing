cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(relive VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_program(CMAKE cmake)
if(NOT CMAKE)
    message(FATAL_ERROR "CMake binary not found!")
endif()

add_custom_target(
    UpdateConfigHeader
    COMMAND "${CMAKE}" -D BUILD_NUMBER=${BUILD_NUMBER} -P ${CMAKE_CURRENT_LIST_DIR}/options.cmake
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

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

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
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/AliveExe)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/Tools/alive_api)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/assets)

if (WIN32)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/AliveDllAE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Source/AliveDllAO)
endif()

add_dependencies(AliveLibCommon UpdateConfigHeader)

if(WIN32)
    # On Windows generate ZIP packages
    SET(CPACK_GENERATOR "ZIP")
endif()

SET(CPACK_PACKAGE_CONTACT "nemin@oddwords.hu")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://aliveteam.github.io/")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0, zenity")

INCLUDE(CPack)
