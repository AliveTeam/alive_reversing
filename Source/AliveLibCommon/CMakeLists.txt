project(AliveLibCommon VERSION 0.1 LANGUAGES CXX)

SET(AliveLibSrcCommon
    BaseGameAutoPlayer.cpp
    BaseGameAutoPlayer.hpp
    PathDataExtensionsTypes.hpp
    CompressionType_4Or5.cpp
    CompressionType_4Or5.hpp
    FG1Reader.hpp
    ExportHooker.hpp
    ExportHooker.cpp
    CamDecompressor.cpp
    CamDecompressor.hpp
    vlctable.hpp
    logger.hpp
    Function.cpp
    Function.hpp
    FunctionFwd.hpp
    Primitives_common.hpp
    Primitives_common.cpp
    Sys_common.hpp
    Sys_common.cpp
    BitField.hpp
    FixedPoint_common.hpp
    AnimResources.hpp
    AnimResources.cpp
    PtrStream.hpp
    Masher.hpp
    Masher.cpp
    masher_tables.hpp
    Types.hpp
    PSXADPCMDecoder.cpp
    PSXADPCMDecoder.h
    PSXMDECDecoder.cpp
    PSXMDECDecoder.h
    Psx_common.hpp
    W32CrashHandler.hpp
    audio/mixer/AliveAudio.hpp
    audio/mixer/AliveAudio.cpp
    audio/mixer/Voice.hpp
    audio/mixer/Voice.cpp
    audio/mixer/ADSR.hpp
    audio/mixer/ADSR.cpp
    audio/Exceptions.hpp
    audio/Stream.hpp
    audio/Stream.cpp
    audio/Soundbank.hpp
    audio/Soundbank.cpp
    audio/SequencePlayer.hpp
    audio/SequencePlayer.cpp
    audio/MidiPlayer.hpp
    audio/MidiPlayer.cpp
    audio/mixer/biquad.hpp
    audio/mixer/biquad.cpp
)

add_library(AliveLibCommon ${AliveLibSrcCommon})

target_include_directories(AliveLibCommon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src ${CMAKE_BINARY_DIR})

target_compile_definitions(AliveLibCommon PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_compile_definitions(AliveLibCommon PRIVATE "BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE")
target_compile_definitions(AliveLibCommon PRIVATE "BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS")

if (MSVC)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_compile_options(AliveLibCommon PRIVATE /W4 /wd4996 /WX /MP)
    else()
        target_compile_options(AliveLibCommon PRIVATE /W4 /wd4996 /MP)
    endif()

    set_target_properties(AliveLibCommon PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /functionpadmin:16")
    set_target_properties(AliveLibCommon PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /functionpadmin:16")
endif()

target_compile_features(AliveLibCommon
    PRIVATE cxx_strong_enums
    PRIVATE cxx_static_assert
    PRIVATE cxx_lambdas
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    PRIVATE cxx_decltype
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)

if (MINGW)
    set(WIN32_LIBS "")
else()
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
endif()

target_link_libraries(AliveLibCommon
    EasyLogging++
    googletest
    Cinder
    libglew_static
    imgui
    glm
    ${WIN32_LIBS}
    ${SDL2_LIBRARIES}
    project_warnings
    )

export(TARGETS AliveLibCommon FILE AliveLibCommon.cmake)
