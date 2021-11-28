if(UNIX)
  SET(BINPATH "bin")
elseif(WIN32)
  SET(BINPATH ".")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lodepng lib, defined here as its the only project that uses it and so that
# users of relive_api don't need to build the whole of relive.
add_library(lodepng_reliveapi
    ../../../3rdParty/lodepng/lodepng.cpp
    ../../../3rdParty/lodepng/lodepng.h
    )

target_include_directories(lodepng_reliveapi SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty>
    $<INSTALL_INTERFACE:lodepng>)

set_property(TARGET lodepng_reliveapi PROPERTY FOLDER "3rdParty")
export(TARGETS lodepng_reliveapi FILE lodepng_reliveapi.cmake)

# jsonxx lib, ditto defining here for relive_api upstream editor projects
add_library(jsonxx_reliveapi ../../../3rdParty/jsonxx/jsonxx.cc)
target_include_directories(jsonxx_reliveapi SYSTEM PUBLIC
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdParty/jsonxx>)
export(TARGETS jsonxx_reliveapi FILE jsonxx_reliveapi.cmake)
set_property(TARGET jsonxx_reliveapi PROPERTY FOLDER "3rdParty")


set(relive_api_src
    ../../../3rdParty/EasyLogging++/EasyLogging/src/easylogging++.cc
    ../../AliveLibAO/PathData.cpp
    ../../AliveLibAE/PathData.cpp
    ../../AliveLibCommon/CamDecompressor.cpp
    CamConverter.hpp
    CamConverter.cpp
    JsonReadUtils.hpp
    JsonModelTypes.hpp
    JsonModelTypes.cpp
    JsonMapRootInfoReader.hpp
    JsonMapRootInfoReader.cpp
    JsonReaderBase.hpp
    JsonReaderBase.cpp
    JsonReaderAE.hpp
    JsonReaderAE.cpp
    JsonReaderAO.hpp
    JsonReaderAO.cpp
    JsonWriterBase.hpp
    JsonWriterBase.cpp
    JsonWriterAE.hpp
    JsonWriterAE.cpp
    JsonWriterAO.hpp
    JsonWriterAO.cpp
    JsonUpgraderAE.cpp
    JsonUpgraderAE.hpp
    TlvsAE.hpp
    JsonUpgraderAO.cpp
    JsonUpgraderAO.hpp
    TlvsAO.hpp
    BaseProperty.cpp
    BaseProperty.hpp
    BasicType.hpp
    BasicTypeBase.cpp
    BasicTypeBase.hpp
    ByteStream.hpp
    EnumType.hpp
    EnumTypeBase.hpp
    ITypeBase.cpp
    ITypeBase.hpp
    JsonUpgrader.cpp
    JsonUpgrader.hpp
    JsonUpgraderMacros.hpp
    LvlReaderWriter.hpp
    PropertyCollection.cpp
    PropertyCollection.hpp
    relive_api_test.cpp
    relive_api.cpp
    relive_api.hpp
    relive_api_exceptions.hpp
    TlvFactory.cpp
    TlvFactory.hpp
    TlvObjectBase.cpp
    TlvObjectBase.hpp
    TlvObjectBaseAE.cpp
    TlvObjectBaseAE.hpp
    TlvObjectBaseAO.cpp
    TlvObjectBaseAO.hpp
    TlvObjectBaseMacros.hpp
    TypedProperty.cpp
    TypedProperty.hpp
    TypesCollectionAE.cpp
    TypesCollectionAE.hpp
    TypesCollectionAO.cpp
    TypesCollectionAO.hpp
    TypesCollectionBase.cpp
    TypesCollectionBase.hpp
)
add_library(relive_api ${relive_api_src})
target_link_libraries(relive_api jsonxx_reliveapi lodepng_reliveapi project_warnings)
export(TARGETS relive_api FILE relive_api.cmake)


add_executable(relive_api_test relive_api_test.cpp)

if (MSVC)
    target_compile_options(relive_api PRIVATE /W4 /wd4996 /WX /MP)
    target_compile_options(relive_api_test PRIVATE /W4 /wd4996 /WX /MP)
endif()

target_include_directories(relive_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/EasyLogging++/EasyLogging/src
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googlemock/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../../3rdParty/googletest/googletest/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})

target_include_directories(relive_api_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../3rdParty/magic_enum/include
    $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_BINARY_DIR})
target_compile_features(relive_api_test
    PRIVATE cxx_auto_type
    PRIVATE cxx_variadic_templates)
target_compile_definitions(relive_api_test PRIVATE "_CRT_SECURE_NO_WARNINGS")
target_link_libraries(relive_api_test relive_api googletest jsonxx_reliveapi libglew_static project_warnings)

export(TARGETS relive_api_test FILE relive_api_test.cmake)
install(TARGETS relive_api_test DESTINATION "${BINPATH}")
