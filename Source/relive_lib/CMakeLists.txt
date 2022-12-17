project(relive_lib VERSION 0.1 LANGUAGES CXX)

SET(relive_lib_src
    # temp hack to use these bits of the api for the data conversion
    ../Tools/relive_api/Base64.cpp
    ../Tools/relive_api/CamConverter.cpp
    ../Tools/relive_api/CamConverter.hpp
    ../Tools/relive_api/ApiFG1Reader.cpp
    ../Tools/relive_api/ApiFG1Reader.hpp
    Renderer/IRenderer.hpp
    Renderer/IRenderer.cpp
    Renderer/Vulkan/VulkanRenderer.hpp
    Renderer/Vulkan/VulkanRenderer.cpp
    Renderer/DirectX9/DirectX9Renderer.hpp
    Renderer/DirectX9/DirectX9Renderer.cpp
    Renderer/PaletteCache.hpp
    Renderer/TextureCache.hpp
    Renderer/OpenGL3/GLContext.hpp
    Renderer/OpenGL3/GLContext.cpp
    Renderer/OpenGL3/GLDebug.hpp
    Renderer/OpenGL3/GLDebug.cpp
    Renderer/OpenGL3/GLFramebuffer.hpp
    Renderer/OpenGL3/GLFramebuffer.cpp
    Renderer/OpenGL3/GLShader.hpp
    Renderer/OpenGL3/GLShader.cpp
    Renderer/OpenGL3/GLShaderProgram.hpp
    Renderer/OpenGL3/GLShaderProgram.cpp
    Renderer/OpenGL3/GLTexture2D.hpp
    Renderer/OpenGL3/GLTexture2D.cpp
    Renderer/OpenGL3/OpenGLRenderer.hpp
    Renderer/OpenGL3/OpenGLRenderer.cpp
    Renderer/OpenGL3/Shaders/ShaderPassthru.cpp
    Renderer/OpenGL3/Shaders/ShaderPsx.cpp
    Sound/PsxSpuApi.cpp
    Sound/PsxSpuApi.hpp
    Sound/Midi.cpp
    Sound/Midi.hpp
    Sound/Sound.cpp
    Sound/Sound.hpp
    Sound/Reverb.hpp
    Sound/Reverb.cpp
    Sound/SoundDSound.cpp
    Sound/SoundDSound.hpp
    Sound/SoundSDL.hpp
    Sound/SoundSDL.cpp
    Sound/SDLSoundBuffer.hpp
    Sound/SDLSoundBuffer.cpp
    Sound/SDLSoundSystem.hpp
    Sound/SDLSoundSystem.cpp
    GameObjects/BaseGameObject.hpp
    GameObjects/BaseGameObject.cpp
    GameObjects/BaseAnimatedWithPhysicsGameObject.hpp
    GameObjects/BaseAnimatedWithPhysicsGameObject.cpp
    GameObjects/EffectBase.hpp
    GameObjects/EffectBase.cpp
    GameObjects/IBaseAliveGameObject.hpp
    GameObjects/IBaseAliveGameObject.cpp
    GameObjects/Flash.hpp
    GameObjects/Flash.cpp
    GameObjects/Particle.cpp
    GameObjects/Particle.hpp
    GameObjects/ScreenManager.cpp
    GameObjects/ScreenManager.hpp
    GameObjects/ShadowZone.hpp
    GameObjects/ShadowZone.cpp
    BinaryPath.hpp
    BinaryPath.cpp
    SwitchStates.hpp
    SwitchStates.cpp
    FG1.hpp
    FG1.cpp
    Camera.cpp
    Camera.hpp
    Sfx.hpp
    Sfx.cpp
    ObjectIds.hpp
    ObjectIds.cpp
    DynamicArray.hpp
    DynamicArray.cpp
    Events.hpp
    Events.cpp
    GameType.hpp
    GameType.cpp
    ResourceManagerWrapper.hpp
    ResourceManagerWrapper.cpp
    MapWrapper.hpp
    MapWrapper.cpp
    PsxDisplay.cpp
    PsxDisplay.hpp
    Compression.hpp
    Compression.cpp
    Math.hpp
    Math.cpp
    Collisions.hpp
    Collisions.cpp
    AnimationBase.cpp
    AnimationBase.hpp
    Primitives.hpp
    Primitives.cpp
    Animation.cpp
    Animation.hpp
    AnimationUnknown.cpp
    AnimationUnknown.hpp
    Shadow.hpp
    Shadow.cpp
    data_conversion/guid.cpp
    data_conversion/guid.hpp
    data_conversion/data_conversion.hpp
    data_conversion/data_conversion.cpp
    data_conversion/file_system.hpp
    data_conversion/file_system.cpp
    data_conversion/AnimationConverter.hpp
    data_conversion/AnimationConverter.cpp
    data_conversion/relive_tlvs.hpp
    data_conversion/relive_tlvs_conversion.hpp
    data_conversion/relive_tlvs_serialization.hpp
    data_conversion/PathTlvsAE.hpp
    data_conversion/PathTlvsAO.hpp
    data_conversion/AnimConversionInfo.hpp
    data_conversion/AnimConversionInfo.cpp
    data_conversion/PNGFile.hpp
    data_conversion/PNGFile.cpp
    data_conversion/AESaveConverter.hpp
    data_conversion/AESaveConverter.cpp
    data_conversion/AESaveSerialization.hpp
    data_conversion/data_conversion_ui.hpp
    data_conversion/data_conversion_ui.cpp
    data_conversion/fmv_converter.cpp
    data_conversion/fmv_converter.hpp
    Engine.hpp
    Engine.cpp
    Layer.hpp
    stdafx.h
    ObjectTypes.hpp
    Clamp.hpp
    Types.hpp
    Masher.hpp
    Masher.cpp
    FatalError.hpp
    FatalError.cpp
    masher_tables.hpp
    RGB16.hpp
    CamDecompressor.cpp
    CamDecompressor.hpp
    BaseGameAutoPlayer.cpp
    BaseGameAutoPlayer.hpp
    PtrStream.hpp
    CompressionType_4Or5.cpp
    CompressionType_4Or5.hpp
    W32CrashHandler.hpp
    BitField.hpp
    FG1Reader.hpp
    PSXMDECDecoder.cpp
    PSXMDECDecoder.h
    PSXADPCMDecoder.cpp
    PSXADPCMDecoder.h
    PathDataExtensionsTypes.hpp
    vlctable.hpp
    AnimResources.hpp
    AnimResources.cpp
    logger.hpp
    AddPointer.hpp
    Function.hpp
    Sys.hpp
    Sys.cpp
    Psx.hpp
    Psx.cpp
    FixedPoint.hpp
    FixedPoint.cpp
)

add_library(relive_lib ${relive_lib_src})

if (MSVC)
# for UuidCreate()
target_link_libraries(relive_lib Rpcrt4)
endif()

target_include_directories(relive_lib PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/aom
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/aom/third_party/libwebm
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/json/single_include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/DX9_SDK_Aug09/Include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src ${CMAKE_BINARY_DIR})

target_compile_definitions(relive_lib PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_compile_definitions(relive_lib PRIVATE "JSON_DISABLE_ENUM_SERIALIZATION=1")

if (MSVC)
    target_compile_options(relive_lib PRIVATE /W4 /wd4996 /WX /MP)
endif()

target_compile_features(relive_lib
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
else()
    if (WIN32)
        set(WIN32_LIBS
            Winmm
            dsound
            xinput
        )
    endif()
endif()

target_link_libraries(relive_lib
    shelf-pack-cpp
    project_warnings
    spng
    miniz
    aom
    webm
    aom_common_app_util
    Cinder
    libglew_static
    imgui
    glm
    volk
    Vulkan::Headers
    ${WIN32_LIBS}
    ${SDL2_LIBRARIES}
    )

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_link_libraries(relive_lib ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/DX9_SDK_Aug09/Lib/x86/D3dx9.lib)
    else()
        target_link_libraries(relive_lib ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/DX9_SDK_Aug09/Lib/x64/D3dx9.lib)
    endif()
endif()

export(TARGETS relive_lib FILE relive_lib.cmake)
