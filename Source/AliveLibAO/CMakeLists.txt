project(AliveLibAO VERSION 0.1 LANGUAGES CXX)

SET(AliveLibSrcAO
   BaseGameObject.hpp
   BaseGameObject.cpp
   DynamicArray.hpp
   DynamicArray.cpp
   Game.hpp
   Game.cpp
   stdlib.hpp
   stdlib.cpp
   EffectBase.hpp
   EffectBase.cpp
   Primitives.hpp
   Primitives.cpp
   Map.hpp
   Map.cpp
   Alarm.hpp
   Alarm.cpp
   WinMain.hpp
   WinMain.cpp
   PathData.hpp
   PathData.cpp
   bmp.hpp
   Animation.hpp
)

ADD_MSVC_PRECOMPILED_HEADER(stdafx_ao.h stdafx_ao.cpp AliveLibSrcAO)

add_library(AliveLibAO ${AliveLibSrcAO})

target_include_directories(AliveLibAO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src ${CMAKE_BINARY_DIR})

target_compile_definitions(AliveLibAO PRIVATE "_CRT_SECURE_NO_WARNINGS")
# Not implemented for AO yet
#target_compile_definitions(AliveLibAO PRIVATE "BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE")
#target_compile_definitions(AliveLibAO PRIVATE "BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS")

if (MSVC)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_compile_options(AliveLibAO PRIVATE /W4 /wd4996 /WX /MP)
    else()
        target_compile_options(AliveLibAO PRIVATE /W4 /wd4996 /MP)
    endif()

    set_target_properties(AliveLibAO PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16")
    set_target_properties(AliveLibAO PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16")
endif()

target_compile_features(AliveLibAO
    PRIVATE cxx_strong_enums
    PRIVATE cxx_static_assert
    PRIVATE cxx_lambdas
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    PRIVATE cxx_decltype
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (WIN32)
    set(WIN32_LIBS
        Winmm
        dsound
        xinput
    )
endif()

if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(WIN32_LIBS ${WIN32_LIBS} Detours)
endif()

target_link_libraries(AliveLibAO
    AliveLibCommon
    EasyLogging++
    googletest
    Cinder
    ${WIN32_LIBS}
    ${SDL2_LIBRARIES}
    )

export(TARGETS AliveLibAO FILE AliveLibAO.cmake)
