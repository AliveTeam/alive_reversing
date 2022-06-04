project(relive_lib VERSION 0.1 LANGUAGES CXX)

SET(relive_lib_src
    ObjectIds.hpp
    ObjectIds.cpp
    BaseGameObject.hpp
    BaseGameObject.cpp
)

add_library(relive_lib ${relive_lib_src})

target_include_directories(AliveLibAE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src ${CMAKE_BINARY_DIR})

target_compile_definitions(AliveLibAE PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_compile_definitions(AliveLibAE PRIVATE "BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE")
target_compile_definitions(AliveLibAE PRIVATE "BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS")

if (MSVC)
    target_compile_options(AliveLibAE PRIVATE /W4 /wd4996 /WX /MP)
endif()

target_compile_features(AliveLibAE
    PRIVATE cxx_strong_enums
    PRIVATE cxx_static_assert
    PRIVATE cxx_lambdas
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    PRIVATE cxx_decltype
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

target_link_libraries(relive_lib
    EasyLogging++
    project_warnings
    )

export(TARGETS relive_lib FILE relive_lib.cmake)
