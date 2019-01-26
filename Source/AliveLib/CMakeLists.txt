project(AliveLib VERSION 0.1 LANGUAGES CXX)

add_library(AliveLib
    stdafx.cpp
    stdafx.h
    ExportHooker.hpp
    ExportHooker.cpp
    logger.hpp
    Function.cpp
    Function.hpp
    Input.cpp
    Input.hpp
    DynamicArray.cpp
    DynamicArray.hpp
    Sys.cpp
    Sys.hpp
    VGA.cpp
    VGA.hpp
    DDraw.cpp
    DDraw.hpp
    Error.cpp
    Error.hpp
    Psx.cpp
    Psx.hpp
    PsxRender.cpp
    PsxRender.hpp
    Math.cpp
    Math.hpp
    Midi.cpp
    Midi.hpp
    GameSpeak.hpp
    GameSpeak.cpp
    DDCheat.hpp
    DDCheat.cpp
    bmp.cpp
    bmp.hpp
    Sfx.hpp
    Sfx.cpp
    Game.cpp
    Game.hpp
    Sound.cpp
    Sound.hpp
    stdlib.cpp
    stdlib.hpp
    ResourceManager.cpp
    ResourceManager.hpp
    BaseGameObject.cpp
    BaseGameObject.hpp
    BaseAliveGameObject.cpp
    BaseAliveGameObject.hpp
    BaseAnimatedWithPhysicsGameObject.cpp
    BaseAnimatedWithPhysicsGameObject.hpp
    CreditsController.cpp
    CreditsController.hpp
    PauseMenu.cpp
    PauseMenu.hpp
    Primitives.cpp
    Primitives.hpp
    Map.cpp
    Map.hpp
    PsxDisplay.cpp
    PsxDisplay.hpp
    ScreenManager.cpp
    ScreenManager.hpp
    vlctable.hpp
    Animation.cpp
    Animation.hpp
    BackgroundAnimation.cpp
    BackgroundAnimation.hpp
    PathData.cpp
    PathData.hpp
    LvlArchive.cpp
    LvlArchive.hpp
    Io.cpp
    Io.hpp
    FixedPoint.hpp
    FixedPoint.cpp
    Resources.hpp
    Resources.cpp
    Font.hpp
    Font.cpp
    RenderingTestTimData.hpp
    DebugHelpers.hpp
    DebugHelpers.cpp
    Events.cpp
    Events.hpp
    SwitchStates.hpp
    SwitchStates.cpp
    StringFormatters.hpp
    StringFormatters.cpp
    Abe.hpp
    Abe.cpp
    MusicController.hpp
    MusicController.cpp
    BackgroundMusic.hpp
    BackgroundMusic.cpp
    Path.hpp
    Path.cpp
    QuikSave.hpp
    QuikSave.cpp
    Factory.hpp
    Factory.cpp
    MainMenu.hpp
    MainMenu.cpp
    LCDScreen.hpp
    LCDScreen.cpp
    StatsSign.hpp
    StatsSign.cpp
    ObjectIds.hpp
    ObjectIds.cpp
    VRam.hpp
    VRam.cpp
    Collisions.hpp
    Collisions.cpp
    CheatController.hpp
    CheatController.cpp
    Compression.hpp
    Compression.cpp
    FG1.hpp
    FG1.cpp
    BitField.hpp
    Particle.hpp
    Particle.cpp
    Movie.hpp
    Movie.cpp
    CameraSwapper.cpp
    CameraSwapper.hpp
    MainMenuTransition.hpp
    MainMenuTransition.cpp
    WinMain.cpp
    WinMain.hpp
    Text.cpp
    Text.hpp
    Shadow.hpp
    Shadow.cpp
    ThrowableArray.cpp
    ThrowableArray.hpp
    UXB.cpp
    UXB.hpp
    Mine.cpp
    Mine.hpp
    TimedMine.cpp
    TimedMine.hpp
    TrapDoor.cpp
    TrapDoor.hpp
    MusicTrigger.cpp
    MusicTrigger.hpp
    ScreenShake.cpp
    ScreenShake.hpp
    InvisibleEffect.cpp
    InvisibleEffect.hpp
    Gibs.cpp
    Gibs.hpp
    Throwable.cpp
    Throwable.hpp
    ThrowableTotalIndicator.hpp
    ThrowableTotalIndicator.cpp
    AbilityRing.cpp
    AbilityRing.hpp
    OrbWhirlWind.cpp
    OrbWhirlWind.hpp
    Blood.cpp
    Blood.hpp
    Masher.hpp
    Masher.cpp
    PullRingRope.hpp
    PullRingRope.cpp
    HoistRocksEffect.hpp
    HoistRocksEffect.cpp
    CircularFade.hpp
    CircularFade.cpp
    EffectBase.hpp
    EffectBase.cpp
    Alarm.hpp
    Alarm.cpp
    DeathFadeOut.hpp
    DeathFadeOut.cpp
    PossessionFlicker.hpp
    PossessionFlicker.cpp
    Flash.hpp
    Flash.cpp
    Door.hpp
    Door.cpp
    DeathBirdParticle.hpp
    DeathBirdParticle.cpp
    Dove.hpp
    Dove.cpp
    WorkWheel.hpp
    WorkWheel.cpp
    StatusLight.hpp
    StatusLight.cpp
    WheelSyncer.hpp
    WheelSyncer.cpp
    LevelLoader.hpp
    LevelLoader.cpp
    InvisibleSwitch.hpp
    InvisibleSwitch.cpp
    Switch.hpp
    Switch.cpp
    MultiSwitchController.hpp
    MultiSwitchController.cpp
    Reverb.hpp
    Reverb.cpp
    SoundSDL.hpp
    SoundSDL.cpp
    ShadowZone.hpp
    ShadowZone.cpp
    Grinder.cpp
    Grinder.hpp
    Teleporter.cpp
    Teleporter.hpp
    Spark.cpp
    Spark.hpp
    Leaf.hpp
    Leaf.cpp
	WinAPISupport.hpp
	WinAPISupport.cpp
    Types.hpp
    Slurg.cpp
    Slurg.hpp)

target_include_directories(AliveLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src ${CMAKE_BINARY_DIR})

target_compile_definitions(AliveLib PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_compile_definitions(AliveLib PRIVATE "BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE")
target_compile_definitions(AliveLib PRIVATE "BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS")

if (MSVC)
    target_compile_options(AliveLib PRIVATE /W4 /wd4996 /WX /MP)

    set_target_properties(AliveLib PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16")
    set_target_properties(AliveLib PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16")
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
    googletest
    Winmm
    dsound
    xinput
    ${SDL2_LIBRARIES}
    )

export(TARGETS AliveLib FILE AliveLib.cmake)
