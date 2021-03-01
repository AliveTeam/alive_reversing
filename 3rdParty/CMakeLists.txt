project(Deps VERSION 0.1 LANGUAGES C CXX)

if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    # Detours lib
    add_library(Detours
        Detours/src/detours.cpp
        Detours/src/disasm.cpp
        Detours/src/image.cpp
        Detours/src/modules.cpp
        )

    target_compile_definitions(Detours PRIVATE "DETOURS_X86" "DETOURS_32BIT")

    target_include_directories(Detours PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Detours/src>
        $<INSTALL_INTERFACE:Detours/src>)

    set_property(TARGET Detours PROPERTY FOLDER "3rdParty")
    export(TARGETS Detours FILE Detours.cmake)
endif()

# Cinder for lock free circular buffer in audio processing
add_library(Cinder INTERFACE)
target_include_directories(Cinder INTERFACE Cinder/)
export(TARGETS Cinder FILE Cinder.cmake)

# EasyLogging++ lib
add_library(EasyLogging++
    EasyLogging++/EasyLogging/src/easylogging++.cc
    EasyLogging++/EasyLogging/src/easylogging++.h
    )

target_include_directories(EasyLogging++ PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EasyLogging++/EasyLogging/src>
    $<INSTALL_INTERFACE:EasyLogging++/EasyLogging/src>)

set_property(TARGET EasyLogging++ PROPERTY FOLDER "3rdParty")
export(TARGETS EasyLogging++ FILE EasyLogging++.cmake)

# DxOld lib
add_library(DxOld INTERFACE)
target_include_directories(DxOld INTERFACE DxOld/)
export(TARGETS DxOld FILE DxOld.cmake)

# jsonxx lib
add_library(jsonxx jsonxx/jsonxx.cc)
target_include_directories(jsonxx PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/jsonxx)
export(TARGETS jsonxx FILE jsonxx.cmake)
set_property(TARGET jsonxx PROPERTY FOLDER "3rdParty")


# googletest lib
add_library(googletest
    googletest/googletest/src/gtest-all.cc
    googletest/googlemock/src/gmock-all.cc
    )

target_include_directories(googletest PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include/gtest;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include/gtest/internal;

    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include/gmock;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include/gmock/internal;
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include/gmock/internal/custom>
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include/gmock>
    )

export(TARGETS googletest FILE googletest.cmake)
set_property(TARGET googletest PROPERTY FOLDER "3rdParty")
