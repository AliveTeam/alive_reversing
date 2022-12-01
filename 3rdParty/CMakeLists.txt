project(Deps VERSION 0.1 LANGUAGES C CXX)

set(ENABLE_DOCS OFF)
set(ENABLE_EXAMPLES OFF)
set(ENABLE_TESTS OFF)
set(ENABLE_TOOLS OFF)
set(CONFIG_ANALYZER OFF)
set(CONFIG_INSPECTION OFF)
set(BUILD_SHARED_LIBS OFF)
set(CONFIG_WEBM_IO ON)
set(ENABLE_TOOLS ON) # required to get webm project
add_definitions(-DNOMINMAX)
set(AOM_TARGET_CPU generic)
add_subdirectory(aom)
set_property(TARGET aom PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_av1_common PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_av1_decoder PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_av1_encoder PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_dsp PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_dsp_common PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_dsp_decoder PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_dsp_encoder PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_mem PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_pc PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_rtcd PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_scale PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_util PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_version PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_version_check PROPERTY FOLDER "3rdParty")
set_property(TARGET av1_rc_qmode PROPERTY FOLDER "3rdParty")
set_property(TARGET dist PROPERTY FOLDER "3rdParty")
set_property(TARGET yuv PROPERTY FOLDER "3rdParty")
set_property(TARGET webm PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_common_app_util PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_decoder_app_util PROPERTY FOLDER "3rdParty")
set_property(TARGET aom_encoder_app_util PROPERTY FOLDER "3rdParty")


# Cinder for lock free circular buffer in audio processing
add_library(Cinder INTERFACE)
target_include_directories(Cinder SYSTEM INTERFACE Cinder/)
export(TARGETS Cinder FILE Cinder.cmake)

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

# miniz lib
add_library(miniz
    miniz/miniz.c
    miniz/miniz.h
    )

target_include_directories(miniz SYSTEM PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/miniz;
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/miniz>
    )

export(TARGETS miniz FILE miniz.cmake)
set_property(TARGET miniz PROPERTY FOLDER "3rdParty")

# spng lib
add_library(spng
    libspng/spng/spng.c
    libspng/spng/spng.h
    )

target_compile_definitions(spng PRIVATE SPNG_USE_MINIZ)
target_compile_definitions(spng PRIVATE SPNG_STATIC)
target_compile_definitions(spng PUBLIC SPNG_STATIC)

target_include_directories(spng SYSTEM PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/libspng;
    ${CMAKE_CURRENT_SOURCE_DIR}/miniz;
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libspng>
    )

export(TARGETS spng FILE spng.cmake)
set_property(TARGET spng PROPERTY FOLDER "3rdParty")

add_library(shelf-pack-cpp INTERFACE)
target_include_directories(shelf-pack-cpp SYSTEM INTERFACE shelf-pack-cpp/include)
export(TARGETS shelf-pack-cpp FILE shelf-pack-cpp.cmake)
