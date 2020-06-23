project(AliveLibAO VERSION 0.1 LANGUAGES CXX)

SET(AliveLibSrcAO
   BellHammer.hpp
   BellHammer.cpp
   Bat.hpp
   Bat.cpp
   Math.hpp
   Math.cpp
   Shadow.hpp
   Shadow.cpp
   Slig.hpp
   Slig.cpp
   SligSpawner.hpp
   SligSpawner.cpp
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
   Animation.cpp
   ResourceManager.hpp
   ResourceManager.cpp
   ScreenManager.hpp
   ScreenManager.cpp
   Factory.hpp
   Factory.cpp
   GasCountDown.hpp
   GasCountDown.cpp
   Font.hpp
   Font.cpp
   Psx.hpp
   Psx.cpp
   Midi.hpp
   Midi.cpp
   BaseAnimatedWithPhysicsGameObject.hpp
   BaseAnimatedWithPhysicsGameObject.cpp
   BaseAliveGameObject.hpp
   BaseAliveGameObject.cpp
   PsxDisplay.hpp
   PsxDisplay.cpp
   FixedPoint.hpp
   FixedPoint.cpp
   AmbientSound.hpp
   AmbientSound.cpp
   FG1.hpp
   FG1.cpp
   Movie.hpp
   Movie.cpp
   BackgroundMusic.hpp
   BackgroundMusic.cpp
   MusicController.hpp
   MusicController.cpp
   CameraSwapper.hpp
   CameraSwapper.cpp
   Particle.hpp
   Particle.cpp
   LvlArchive.hpp
   LvlArchive.cpp
   Collisions.hpp
   Collisions.cpp
   Events.hpp
   Events.cpp
   Input.hpp
   Input.cpp
   GameSpeak.hpp
   GameSpeak.cpp
   Abe.hpp
   Abe.cpp
   PauseMenu.hpp
   PauseMenu.cpp
   DDCheat.hpp
   DDCheat.cpp
   CheatController.hpp
   CheatController.cpp
   RollingBallStopper.hpp
   RollingBallStopper.cpp
   SwitchStates.hpp
   SwitchStates.cpp
   Sfx.hpp
   Sfx.cpp
   ThrowableArray.hpp
   ThrowableArray.cpp
   Elum.hpp
   Elum.cpp
   ZBall.hpp
   ZBall.cpp
   RollingBallShaker.hpp
   RollingBallShaker.cpp
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
