project(AliveLib VERSION 0.1 LANGUAGES CXX)

add_library(AliveLib
    stdafx.cpp
    stdafx.h
    logger.hpp
    Function.cpp
    Function.hpp
    WinMain.cpp
    WinMain.hpp)

target_include_directories(AliveLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_compile_definitions(AliveLib PRIVATE "_CRT_SECURE_NO_WARNINGS")

if (MSVC)
    target_compile_options(AliveLib PRIVATE /W4 /wd4996)
endif()

SET_TARGET_PRECOMPILED_HEADER(AliveLib stdafx.h stdafx.cpp)

target_compile_features(AliveLib
    PRIVATE cxx_strong_enums
    PRIVATE cxx_static_assert
    PRIVATE cxx_lambdas
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    PRIVATE cxx_decltype
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_link_libraries(AliveLib
    Detours
    EasyLogging++
    )

export(TARGETS AliveLib FILE AliveLib.cmake)
