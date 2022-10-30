project(relive_lib VERSION 0.1 LANGUAGES CXX)

SET(relive_lib_src
    # temp hack to use these bits of the api for the data conversion
    ../Tools/relive_api/Base64.cpp
    ../Tools/relive_api/CamConverter.cpp
    ../Tools/relive_api/CamConverter.hpp
    ../Tools/relive_api/ApiFG1Reader.cpp
    ../Tools/relive_api/ApiFG1Reader.hpp
    IBaseAliveGameObject.hpp
    IBaseAliveGameObject.cpp
    BinaryPath.hpp
    BinaryPath.cpp
    ShadowZone.hpp
    ShadowZone.cpp
    FG1.hpp
    FG1.cpp
    VRam.hpp
    VRam.cpp
    Camera.cpp
    Camera.hpp
    Sfx.hpp
    Sfx.cpp
    ObjectIds.hpp
    ObjectIds.cpp
    BaseGameObject.hpp
    BaseGameObject.cpp
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
    ScreenManager.cpp
    ScreenManager.hpp
    Math.hpp
    Math.cpp
    Collisions.hpp
    Collisions.cpp
    BaseAnimatedWithPhysicsGameObject.hpp
    BaseAnimatedWithPhysicsGameObject.cpp
    Particle.cpp
    Particle.hpp
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
    Layer.hpp
    stdafx.h
    ObjectTypes.hpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/json/single_include>
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

target_link_libraries(relive_lib
    EasyLogging++
    project_warnings
    spng
    miniz
    )

export(TARGETS relive_lib FILE relive_lib.cmake)
