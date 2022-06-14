project(Deps VERSION 0.1 LANGUAGES C CXX)

# Cinder for lock free circular buffer in audio processing
add_library(Cinder INTERFACE)
target_include_directories(Cinder SYSTEM INTERFACE Cinder/)
export(TARGETS Cinder FILE Cinder.cmake)

# EasyLogging++ lib
add_library(EasyLogging++
    EasyLogging++/EasyLogging/src/easylogging++.cc
    EasyLogging++/EasyLogging/src/easylogging++.h
    )

target_include_directories(EasyLogging++ SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EasyLogging++/EasyLogging/src>
    $<INSTALL_INTERFACE:EasyLogging++/EasyLogging/src>)

set_property(TARGET EasyLogging++ PROPERTY FOLDER "3rdParty")
export(TARGETS EasyLogging++ FILE EasyLogging++.cmake)

# DxOld lib
add_library(DxOld INTERFACE)
target_include_directories(DxOld SYSTEM INTERFACE DxOld/)
export(TARGETS DxOld FILE DxOld.cmake)

# googletest lib
add_library(googletest
    googletest/googletest/src/gtest-all.cc
    googletest/googlemock/src/gmock-all.cc
    )

target_include_directories(googletest SYSTEM PUBLIC
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

# dear imgui lib
add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_sdl.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    )

target_link_libraries(imgui
    libglew_static
    ${SDL2_LIBRARIES}
    )

target_include_directories(imgui SYSTEM PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui;
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
    )
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLEW)
export(TARGETS imgui FILE imgui.cmake)
set_property(TARGET imgui PROPERTY FOLDER "3rdParty")

option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glew-cmake)
set_property(TARGET libglew_static PROPERTY FOLDER "3rdParty")
export(TARGETS libglew_static FILE libglew_static.cmake)

# glm
add_library(glm INTERFACE)

target_include_directories(glm SYSTEM INTERFACE
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/glm>
    )
export(TARGETS glm FILE glm.cmake)
