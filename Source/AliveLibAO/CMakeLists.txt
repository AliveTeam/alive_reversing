project(AliveLibAO VERSION 0.1 LANGUAGES CXX)

SET(AliveLibSrcAO
   Font.hpp
   Font.cpp
   Midi.hpp
   Midi.cpp
   Sfx.hpp
   Sfx.cpp
   Math.hpp
   Math.cpp
   Grid.cpp
   Grid.hpp
   StringFormatters.hpp
   StringFormatters.cpp
   BaseAliveGameObject.hpp
   BaseAliveGameObject.cpp
   GameAutoPlayer.cpp
   GameAutoPlayer.hpp
   PathDataExtensions.hpp
   PathDataExtensions.cpp
   Game.hpp
   Game.cpp
   Map.hpp
   Map.cpp
   Sound.hpp
   Sound.cpp
   SaveGame.hpp
   SaveGame.cpp
   CameraSwapper.hpp
   CameraSwapper.cpp
   Input.hpp
   Input.cpp
   PauseMenu.hpp
   PauseMenu.cpp
   Path.hpp
   Path.cpp
   DDCheat.hpp
   DDCheat.cpp
   DemoPlayback.hpp
   DemoPlayback.cpp
   SwitchStateBooleanLogic.hpp
   SwitchStateBooleanLogic.cpp
   OneShotSwitchIdSetter.hpp
   OneShotSwitchIdSetter.cpp
   GameEnderController.hpp
   GameEnderController.cpp
   MovingBomb.hpp
   MovingBomb.cpp
   ScreenWave.hpp
   ScreenWave.cpp
   UXB.hpp
   UXB.cpp
   LCDScreen.hpp
   LCDScreen.cpp
   MainMenu.hpp
   MainMenu.cpp
   MainMenuTransition.hpp
   MainMenuTransition.cpp
   Mine.hpp
   Mine.cpp
   Door.hpp
   Door.cpp
   HintFly.hpp
   HintFly.cpp
   SnoozeParticle.hpp
   SnoozeParticle.cpp
   ScopedSeq.hpp
   ScopedSeq.cpp
   TrapDoor.hpp
   TrapDoor.cpp
   MeatSaw.hpp
   MeatSaw.cpp
   Grenade.hpp
   Grenade.cpp
   FlintLockFire.hpp
   FlintLockFire.cpp
   SlingMudokon.hpp
   SlingMudokon.cpp
   Mudokon.hpp
   Mudokon.cpp
   Slig.hpp
   Slig.cpp
   Slog.hpp
   Slog.cpp
   Scrab.hpp
   Scrab.cpp
   Paramite.hpp
   Paramite.cpp
   LCDStatusBoard.hpp
   LCDStatusBoard.cpp
   ParamiteWeb.hpp
   ParamiteWeb.cpp
   ZapLine.hpp
   ZapLine.cpp
   Shrykull.hpp
   Shrykull.cpp
   GasEmitter.hpp
   GasEmitter.cpp
   BoomMachine.hpp
   BoomMachine.cpp
   DeathGas.hpp
   DeathGas.cpp
   DoorFlame.hpp
   DoorFlame.cpp
   SlogSpawner.hpp
   SlogSpawner.cpp
   AbilityRing.hpp
   AbilityRing.cpp
   BirdPortal.hpp
   BirdPortal.cpp
   ZzzSpawner.hpp
   ZzzSpawner.cpp
   OrbWhirlWindParticle.hpp
   OrbWhirlWindParticle.cpp
   OrbWhirlWind.hpp
   OrbWhirlWind.cpp
   Well.hpp
   Well.cpp
   Throwable.hpp
   Throwable.cpp
   Rock.hpp
   Rock.cpp
   RockSack.hpp
   RockSack.cpp
   BackgroundGlukkon.hpp
   BackgroundGlukkon.cpp
   ThrowableTotalIndicator.hpp
   ThrowableTotalIndicator.cpp
   CircularFade.hpp
   CircularFade.cpp
   SecurityDoor.hpp
   SecurityDoor.cpp
   GroundExplosion.hpp
   GroundExplosion.cpp
   Lever.hpp
   Lever.cpp
   RollingBall.hpp
   RollingBall.cpp
   Leaf.hpp
   Leaf.cpp
   DeathBirdParticle.hpp
   DeathBirdParticle.cpp
   PossessionFlicker.hpp
   PossessionFlicker.cpp
   HoistRocksEffect.hpp
   HoistRocksEffect.cpp
   FootSwitch.hpp
   FootSwitch.cpp
   FallingItem.hpp
   FallingItem.cpp
   ParticleBurst.hpp
   ParticleBurst.cpp
   InvisibleSwitch.hpp
   InvisibleSwitch.cpp
   Rope.hpp
   Rope.cpp
   PullRingRope.hpp
   PullRingRope.cpp
   BulletShell.hpp
   BulletShell.cpp
   TimerTrigger.hpp
   TimerTrigger.cpp
   BellSong.hpp
   BellSong.cpp
   HoneySack.hpp
   HoneySack.cpp
   Honey.hpp
   Honey.cpp
   HoneyDrip.hpp
   HoneyDrip.cpp
   BeeNest.hpp
   BeeNest.cpp
   BeeSwarmHole.hpp
   BeeSwarmHole.cpp
   BeeSwarm.hpp
   BeeSwarm.cpp
   Fade.hpp
   Fade.cpp
   Gibs.hpp
   Gibs.cpp
   ScreenShake.hpp
   ScreenShake.cpp
   AirExplosion.hpp
   AirExplosion.cpp
   MotionDetector.hpp
   MotionDetector.cpp
   SecurityClaw.hpp
   SecurityClaw.cpp
   SecurityOrb.hpp
   SecurityOrb.cpp
   Electrocute.hpp
   Electrocute.cpp
   MusicTrigger.hpp
   MusicTrigger.cpp
   Dove.hpp
   Dove.cpp
   ElectricWall.hpp
   ElectricWall.cpp
   Meat.hpp
   Meat.cpp
   MeatSack.hpp
   MeatSack.cpp
   CreditsController.hpp
   CreditsController.cpp
   ScreenClipper.hpp
   ScreenClipper.cpp
   Bells.hpp
   Bells.cpp
   ChimeLock.hpp
   ChimeLock.cpp
   ZapSpark.hpp
   ZapSpark.cpp
   Spark.hpp
   Spark.cpp
   Bullet.hpp
   Bullet.cpp
   PlatformBase.hpp
   PlatformBase.cpp
   LiftPoint.hpp
   LiftPoint.cpp
   TimedMine.hpp
   TimedMine.cpp
   Blood.hpp
   Blood.cpp
   LightEffect.hpp
   LightEffect.cpp
   DoorLight.hpp
   DoorLight.cpp
   BackgroundAnimation.hpp
   BackgroundAnimation.cpp
   LiftMover.hpp
   LiftMover.cpp
   BellHammer.hpp
   BellHammer.cpp
   Bat.hpp
   Bat.cpp
   Slig.hpp
   Slig.cpp
   SligSpawner.hpp
   SligSpawner.cpp
   Alarm.hpp
   Alarm.cpp
   PathData.hpp
   PathData.cpp
   AnimationCallBacks.cpp
   AnimationCallBacks.hpp
   ResourceManager.hpp
   ResourceManager.cpp
   Factory.hpp
   Factory.cpp
   GasCountDown.hpp
   GasCountDown.cpp
   AmbientSound.hpp
   AmbientSound.cpp
   Movie.hpp
   Movie.cpp
   BackgroundMusic.hpp
   BackgroundMusic.cpp
   MusicController.hpp
   MusicController.cpp
   CheatController.hpp
   CheatController.cpp
   GameSpeak.hpp
   GameSpeak.cpp
   Abe.hpp
   Abe.cpp
   RollingBallStopper.hpp
   RollingBallStopper.cpp
   ThrowableArray.hpp
   ThrowableArray.cpp
   Elum.hpp
   Elum.cpp
   ZBall.hpp
   ZBall.cpp
   RollingBallShaker.hpp
   RollingBallShaker.cpp
)

add_library(AliveLibAO ${AliveLibSrcAO})

target_include_directories(AliveLibAO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR},
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/json/single_include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src ${CMAKE_BINARY_DIR})

target_compile_definitions(AliveLibAO PRIVATE "_CRT_SECURE_NO_WARNINGS")

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

if (MINGW)
    set(WIN32_LIBS "imm32")
    set(AE_LIB AliveLibAE)
else()
    if (WIN32)
        set(WIN32_LIBS
            Winmm
            dsound
            xinput
        )
    endif()
endif()

target_link_libraries(AliveLibAO
    AliveLibAE
    googletest
    Cinder
    libglew_static
    imgui
    glm
    ${WIN32_LIBS}
    ${SDL2_LIBRARIES}
    project_warnings
    )

export(TARGETS AliveLibAO FILE AliveLibAO.cmake)
